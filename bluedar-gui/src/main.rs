use iced::{mouse, Center, Color, Fill, Point, Rectangle, Renderer, Size, Theme};
use iced::widget::{button, canvas, column, text, Column};

pub fn main() -> iced::Result {
    iced::run("Bluedar GUI", Application::update, Application::view)
}

#[derive(Debug, Clone, Copy)]
enum Message {
}

#[derive(Debug)]
struct Radar {
}

const GRID_LINE_THICKNESS: f32 = 1.0;
const GRID_LINE_SPACING: usize = 20;

impl Radar {
    fn draw_bg_grid(&self, frame: &mut canvas::Frame, bounds: &Rectangle) {
        let bg = canvas::Path::rectangle(Point::new(0.0, 0.0), bounds.size());

        frame.fill(&bg, Color::WHITE);

        for x in (bounds.x as usize..(bounds.x + bounds.width) as usize).step_by(GRID_LINE_SPACING) {
            let line = canvas::Path::rectangle(Point::new(x as f32 - GRID_LINE_THICKNESS / 2.0, 0.0), Size::new(GRID_LINE_THICKNESS, bounds.height));

            frame.fill(&line, Color::from_rgb(0.3, 0.3, 0.3));
        }

        for y in (bounds.y as usize..(bounds.y + bounds.height) as usize).step_by(GRID_LINE_SPACING) {
            let line = canvas::Path::rectangle(Point::new(0.0, y as f32 - GRID_LINE_THICKNESS / 2.0), Size::new(bounds.width, GRID_LINE_THICKNESS));

            frame.fill(&line, Color::from_rgb(0.3, 0.3, 0.3));
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

        vec![frame.into_geometry()]
    }
}

#[derive(Default)]
struct Application {
}

impl Application {
    fn update(&mut self, message: Message) {
    }

    fn view(&self) -> Column<Message> {
        column![
            canvas(Radar { })
            .width(Fill)
            .height(Fill)
        ]
        .width(Fill)
        .height(Fill)
        .padding(5)
        .align_x(Center)
    }
}
