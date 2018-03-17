#include <iostream>
#include "gethin.hpp"

void drawRectangle(int width, int height);
void drawEllipse(int width, int height);

int main(int argc, char* argv[]) {
  gethin::String width = gethin::String()
                             .shortOpt('x')
                             .longOpt("width")
                             .mandatory(true)
                             .name("width")
                             .help("The width of the drawn shape.");

  gethin::String height = gethin::String()
                              .shortOpt('y')
                              .longOpt("height")
                              .mandatory(true)
                              .name("height")
                              .help("The height of the shape.");

  gethin::Flag color = gethin::Flag().shortOpt('c').longOpt("colors").help(
      "Draw the shape with colored output.");

  gethin::Set shape = gethin::Set()
                          .shortOpt('s')
                          .longOpt("shape")
                          .mandatory(true)
                          .alternatives({"rectangle", "ellipsis"})
                          .help("Which shape to draw.");

  gethin::OptionReader optReader({&width, &height, &color, &shape});
  try {
    optReader.read(argc, argv);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Error during execution!" << std::endl;
  }

  if (shape.value() == "rectangle") {
    drawRectangle(std::stoi(width.value()), std::stoi(height.value()));
  } else if (shape.value() == "ellipsis") {
    drawEllipse(std::stoi(width.value()), std::stoi(height.value()));
  }
}

void drawRectangle(int width, int height) {
  for (int x = 0; x < width; ++x) {
    for (int y = 0; y < height; ++y) {
      std::cout << "#";
    }
    std::cout << std::endl;
  }
}

void drawEllipse(int width, int height) {
  int h2 = height * height;
  int w2 = width * width;
  for (int y = -height; y <= height; y++) {
    for (int x = -width; x <= width; x++) {
      if (x * x * h2 + y * y * w2 <= h2 * w2) {
        std::cout << "#";
        // setpixel(origin.x + x, origin.y + y);
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}