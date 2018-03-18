#include <iostream>
#include "gethin.hpp"

void drawRectangle(int width, int height);
void drawEllipse(int width, int height);

int main(int argc, char* argv[]) {
  gethin::String width = gethin::String()
                             .shortOpt('x')
                             .longOpt("width")
                             .mandatory()
                             .name("width")
                             .help("The width of the drawn shape.");

  gethin::String height = gethin::String()
                              .shortOpt('y')
                              .longOpt("height")
                              .mandatory()
                              .name("height")
                              .help("The height of the shape.");

  gethin::Flag color = gethin::Flag().shortOpt('c').longOpt("colors").help(
      "Draw the shape with colored output.");

  gethin::Set shape = gethin::Set()
                          .shortOpt('s')
                          .longOpt("shape")
                          .mandatory()
                          .alternatives({"rectangle", "ellipse"})
                          .help("Which shape to draw.");

  gethin::OptionReader optReader({&width, &height, &color, &shape}, "drawshape");
  try {
    optReader.read(argc, argv);
  } catch (const std::invalid_argument& e) {
    std::cerr << e.what() << std::endl;
  } catch (...) {
    std::cerr << "Error during execution!" << std::endl;
  }

  if (color.value()) {
    std::cout << "\x1B[32m";
  }

  if (shape.value() == "rectangle") {
    drawRectangle(std::stoi(width.value()), std::stoi(height.value()));
  } else if (shape.value() == "ellipse") {
    drawEllipse(std::stoi(width.value()), std::stoi(height.value()));
  }
}

void drawRectangle(const int width, const int height) {
  for (int y = 0; y < height; ++y) {
    for (int x = 0; x < width; ++x) {
      std::cout << "#";
    }
    std::cout << std::endl;
  }
}

void drawEllipse(const int width, const int height) {
  const int wRadius = width/2;
  const int hRadius = height/2;
  const int hr2 = hRadius * hRadius;
  const int wr2 = wRadius * wRadius;
  for (int y = -hRadius; y <= hRadius; y++) {
    for (int x = -wRadius; x <= wRadius; x++) {
      if (x * x * hr2 + y * y * wr2 <= hr2 * wr2) {
        std::cout << "#";
      } else {
        std::cout << " ";
      }
    }
    std::cout << std::endl;
  }
}