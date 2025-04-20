use std::time::Duration;

use iced::{
    futures::stream::Stream,
    mouse, Center, Color, Fill, Point, Rectangle, Renderer, Size, Subscription, Theme};
use iced::widget::{button, canvas, column, text, Column};
use rumqttc::{MqttOptions, AsyncClient, QoS};

fn mqtt_channel() -> impl Stream<Item = Message> {
    iced::stream::channel(100, |mut output| async move {
        let mut mqttoptions = MqttOptions::new("bluedar-gui-test", "broker.emqx.io", 1883);
        mqttoptions.set_keep_alive(Duration::from_secs(5));

        println!("Connecting to MQTT broker...");
        let (mut client, mut eventloop) = AsyncClient::new(mqttoptions, 10);
        client.subscribe("bluedar-test-3", QoS::AtMostOnce).await.unwrap();
        println!("Subscribed to topic.");

        while let Ok(message) = eventloop.poll().await {
            if let rumqttc::Event::Incoming(rumqttc::Incoming::Publish(message)) = message {
                println!("{:?}", message.payload);
            }
        }
    })
}

pub fn main() -> iced::Result {
    iced::application("Bluedar GUI", Application::update, Application::view)
    .subscription(Application::subscription)
    .run()
}

#[derive(Debug, Clone)]
struct Device {
    mac_address: [u8; 6],
    display_name: Option<String>,
    coords: (f32, f32),
}

#[derive(Debug, Clone)]
enum Message {
}

#[derive(Debug)]
struct Radar {
    devices: Vec<Device>,
}

const GRID_LINE_THICKNESS: f32 = 1.0;
const GRID_LINE_SPACING: usize = 20;
const DEVICE_CIRCLE_RADIUS: f32 = 20.0;

impl Radar {
    fn device_color(device: &Device) -> Color {
        Color::from_rgb8(
            device.mac_address[0] ^ device.mac_address[3],
            device.mac_address[1] ^ device.mac_address[4],
            device.mac_address[2] ^ device.mac_address[5]
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

    fn draw_devices(&self, frame: &mut canvas::Frame, _bounds: &Rectangle) {
        let devices = [
            Device { mac_address: [0x22, 0xb4, 0xcd, 0x50, 0xa7, 0xda], display_name: None, coords: (500.0, 500.0)},
            Device { mac_address: [0x51, 0xd2, 0xce, 0x49, 0x50, 0x93], display_name: None, coords: (300.0, 700.0)}
        ];

        for device in devices {
            let device_circle = canvas::Path::circle(Point::new(device.coords.0, device.coords.1), DEVICE_CIRCLE_RADIUS);

            frame.fill(&device_circle, Radar::device_color(&device));
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
        self.draw_devices(&mut frame, &bounds);

        vec![frame.into_geometry()]
    }
}

#[derive(Default)]
struct Application {
    devices: Vec<Device>,
}

impl Application {
    fn subscription(&self) -> Subscription<Message> {
        Subscription::run(mqtt_channel)
    }

    fn update(&mut self, message: Message) {
    }

    fn view(&self) -> Column<Message> {
        column![
            canvas(Radar { devices: self.devices.clone() })
            .width(Fill)
            .height(Fill)
        ]
        .width(Fill)
        .height(Fill)
        .padding(5)
        .align_x(Center)
    }
}
