//
// Created by simon on 12/1/20.
//

#include "Plugin.hpp"
#include "Schnittstelle.hpp"

void Plugin::print(const std::string &message) {
    Schnittstelle::gui_print(message);
}

void Plugin::draw_rect(TGraphicRect rect) {
    Schnittstelle::gui_draw_rect(rect);
}

void Plugin::draw_circle(TGraphicCircle circle) {
    Schnittstelle::gui_draw_circle(circle);
}

void Plugin::draw_line(TGraphicLine line) {
    Schnittstelle::gui_draw_line(line);
}

void Plugin::draw_text(TGraphicText text) {
    Schnittstelle::gui_draw_text(text);
}

void Plugin::draw_pixel(TGraphicPixel pixel) {
    Schnittstelle::gui_draw_pixel(pixel);
}

void Plugin::clear() {
    Schnittstelle::gui_clear();
}

void Plugin::on_error(int line, const std::string &message) {
    Schnittstelle::on_error(line, message);
}

std::string Plugin::get_input_line(){
    return Schnittstelle::get_input_line();
}
