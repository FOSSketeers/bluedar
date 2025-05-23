use std::collections::HashMap;
use std::time::Duration;

use iced::{
    futures::{sink::SinkExt, stream::Stream},
    mouse, Center, Color, Fill, Point, Rectangle, Renderer, Size, Subscription, Theme};
use iced::widget::{button, canvas, column, text, Column};
use nalgebra::{Matrix1x2, Matrix1x3, Matrix2x3, Matrix3x2, Vector2, Vector3};
use rumqttc::{MqttOptions, AsyncClient, QoS};
use serde::Deserialize;

const PROPAGATION_CONST: f64 = 2.7;
const BIAS: f64 = -59.75;

fn rssi_to_distance(rssi: f64) -> f64 {
    10f64.powf((rssi / (-10.0 * PROPAGATION_CONST)) - BIAS)
}

fn mqtt_channel() -> impl Stream<Item = Message> {
    iced::stream::channel(100, |mut output| async move {
        let mut mqttoptions = MqttOptions::new("bluedar-gui-test", "ikolomiko.com", 1883);
        mqttoptions.set_keep_alive(Duration::from_secs(5));

        println!("Connecting to MQTT broker...");
        let (mut client, mut eventloop) = AsyncClient::new(mqttoptions, 10);
        client.subscribe("bluedar-topic-discovery", QoS::AtMostOnce).await.unwrap();
        println!("Subscribed to topic.");

        while let Ok(message) = eventloop.poll().await {
            if let rumqttc::Event::Incoming(rumqttc::Incoming::Publish(message)) = message {
                let scan: ScanResult = serde_json::from_slice(&message.payload).unwrap();
                println!("DEBUG: Received new scan: {:?}", scan);
                let _ = output.send(Message::NewScan(scan)).await;
            }
        }
    })
}

pub fn main() -> iced::Result {
    iced::application("Bluedar GUI", Application::update, Application::view)
    .subscription(Application::subscription)
    .run()
}

#[derive(Debug, Deserialize, Clone)]
struct Device {
    address: String,
    name: String,
    rssi: i8,
}

#[derive(Debug, Deserialize, Clone)]
struct ScanResult {
    probe_id: i8,
    discovered_devices: Vec<Device>,
}

#[derive(Debug, Clone)]
enum Message {
    NewScan(ScanResult),
}

#[derive(Clone, Debug, Default)]
struct Probe {
    id: u8,
    coords: Matrix1x2<f64>,
}

#[derive(Clone, Debug)]
struct Radar {
    probes: [Probe; 4],
    last_scans: [Option<ScanResult>; 4],
}

// 1m -> 200px
impl Default for Radar {
    fn default() -> Self {
        Radar {
            probes: [
                Probe { id: 1, coords: Matrix1x2::new(300.0, 300.0) },
                Probe { id: 2, coords: Matrix1x2::new(500.0, 300.0) },
                Probe { id: 3, coords: Matrix1x2::new(500.0, 500.0) },
                Probe { id: 4, coords: Matrix1x2::new(300.0, 500.0) },
            ],
            last_scans: [None, None, None, None],
        }
    }
}

const GRID_LINE_THICKNESS: f32 = 1.0;
const GRID_LINE_SPACING: usize = 20;
const DEVICE_CIRCLE_RADIUS: f32 = 15.0;

impl Radar {
    fn device_color() -> Color {
        Color::from_rgb8(
            255, 0, 0
        )
    }

    fn draw_bg_grid(&self, frame: &mut canvas::Frame, bounds: &Rectangle) {
        let bg = canvas::Path::rectangle(Point::new(0.0, 0.0), bounds.size());

        frame.fill(&bg, Color::WHITE);

        for x in (0..bounds.width as usize).step_by(GRID_LINE_SPACING) {
            let line = canvas::Path::rectangle(Point::new(x as f32 - GRID_LINE_THICKNESS / 2.0, 0.0), Size::new(GRID_LINE_THICKNESS, bounds.height));

            frame.fill(&line, Color::from_rgb(0.3, 0.3, 0.3));
        }

        for y in (0..bounds.height as usize).step_by(GRID_LINE_SPACING) {
            let line = canvas::Path::rectangle(Point::new(0.0, y as f32 - GRID_LINE_THICKNESS / 2.0), Size::new(bounds.width, GRID_LINE_THICKNESS));

            frame.fill(&line, Color::from_rgb(0.3, 0.3, 0.3));
        }
    }

    fn draw_probes(&self, frame: &mut canvas::Frame, _bounds: &Rectangle) {
        for probe in &self.probes {
            let circle = canvas::Path::circle(Point::new(probe.coords.x as f32, probe.coords.y as f32), DEVICE_CIRCLE_RADIUS);

            frame.fill(&circle, Color::from_rgb8(0, 0, 0));
        }
    }

    fn draw_devices(&self, frame: &mut canvas::Frame, _bounds: &Rectangle) {
        // ASSUMPTION: A device does not appear multiple times in a single probe.
        let mut devices = HashMap::new();

        // Transposing the scans here, as we're more interested in what probes got a single device rather
        // than what devices a single probe got.
        for scan in &self.last_scans {
            if let Some(scan) = scan {
                for device in &scan.discovered_devices {
                    if !devices.contains_key(&device.address) {
                        devices.insert(&device.address, vec![]);
                    }

                    devices.get_mut(&device.address).unwrap().push(((&self.probes[(scan.probe_id - 1) as usize]).clone(), device.rssi));
                }
            }
        }

        for (address, probes) in devices {
            if probes.len() < 4 {
                // println!("Skipping device {:#?}, does not have enough probes.", address);
                continue;
            }

            let probe_n = (&probes[3]).clone();

            let A = Matrix3x2::from_rows(&[
                2.0 * (probes[0].0.coords - probe_n.0.coords),
                2.0 * (probes[1].0.coords - probe_n.0.coords),
                2.0 * (probes[2].0.coords - probe_n.0.coords),
            ]);

            let sq = |v: &Matrix1x2<f64>| v.x.powi(2) + v.y.powi(2);

            let d_n_sq = rssi_to_distance(probe_n.1 as f64).powi(2);

            let b = Vector3::new(
                sq(&probes[0].0.coords) - sq(&probe_n.0.coords) + d_n_sq - rssi_to_distance(probes[0].1 as f64).powi(2),
                sq(&probes[1].0.coords) - sq(&probe_n.0.coords) + d_n_sq - rssi_to_distance(probes[1].1 as f64).powi(2),
                sq(&probes[2].0.coords) - sq(&probe_n.0.coords) + d_n_sq - rssi_to_distance(probes[2].1 as f64).powi(2),
            );

            let device_coords = (A.transpose() * A).try_inverse().unwrap() * (A.transpose() * b);

            println!("{} {}", address, device_coords);

            let device_circle = canvas::Path::circle(Point::new(device_coords.x as f32, device_coords.y as f32), DEVICE_CIRCLE_RADIUS);

            frame.fill(&device_circle, Radar::device_color());
        }
    }
}

impl<Message> canvas::Program<Message> for Radar {
    type State = ();

    fn draw(
        &self,
        _state: &(),
        renderer: &Renderer,
        _theme: &Theme,
        bounds: Rectangle,
        _cursor: mouse::Cursor
    ) -> Vec<canvas::Geometry> {
        let mut frame = canvas::Frame::new(renderer, bounds.size());

        self.draw_bg_grid(&mut frame, &bounds);
        self.draw_probes(&mut frame, &bounds);
        self.draw_devices(&mut frame, &bounds);

        vec![frame.into_geometry()]
    }
}

#[derive(Default)]
struct Application {
    radar: Radar,
}

impl Application {
    fn subscription(&self) -> Subscription<Message> {
        Subscription::run(mqtt_channel)
    }

    fn update(&mut self, message: Message) {
        match message {
            Message::NewScan(scan) => {
                self.radar.last_scans[scan.probe_id as usize - 1] = Some(scan.clone());
            }
        }
    }

    fn view(&self) -> Column<Message> {
        column![
            canvas(self.radar.clone())
            .width(Fill)
            .height(Fill)
        ]
        .width(Fill)
        .height(Fill)
        .padding(5)
        .align_x(Center)
    }
}
