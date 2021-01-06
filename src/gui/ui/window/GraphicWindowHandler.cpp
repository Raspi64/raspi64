//
// Created by Alex on 15.11.20.
//

#include "GraphicWindowHandler.hpp"
#include "base/WindowHandler.hpp"


GraphicWindowHandler::GraphicWindowHandler(GraphicWindow *window): WindowHandler(window), window(window) {}

void GraphicWindowHandler::add_rect(TGraphicRect rect) {
    window->add_rect(rect);
}

void GraphicWindowHandler::add_circle(TGraphicCircle circle) {
    window->add_circle(circle);
}

void GraphicWindowHandler::add_line(TGraphicLine line) {
    window->add_line(line);
}

void GraphicWindowHandler::add_text(TGraphicText text) {
    window->add_text(text);
}

void GraphicWindowHandler::add_pixel(TGraphicPixel pixel) {
    window->add_pixel(pixel);
}
void GraphicWindowHandler::add_pixel(int x, int y, int red, int green, int blue, int alpha, int size) {
    window->add_pixel(x, y, red, green, blue, alpha, size);
}

void GraphicWindowHandler::clear_pixels() {
    window->clear_pixels();
}
