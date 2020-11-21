//
// Created by Alex on 15.11.20.
//

#include "GraphicWindowHandler.hpp"
#include "base/WindowHandler.hpp"


GraphicWindowHandler::GraphicWindowHandler(GraphicWindow *window): WindowHandler(window), window(window) {}

void GraphicWindowHandler::add_pixel(int x, int y, int red, int green, int blue, int alpha, int size) {
    window->add_pixel(x, y, red, green, blue, alpha, size);
}

void GraphicWindowHandler::clear_pixels() {
    window->clear_pixels();
}
