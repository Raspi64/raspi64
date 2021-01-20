//
// Created by simon on 12/1/20.
//

#include "Plugin.hpp"
#include "Kommunikationsstelle.hpp"

void Plugin::print(const std::string &message) {
    Kommunikationsstelle::gui_print(message);
}

void Plugin::draw_rect(TGraphicRect rect) {
    Kommunikationsstelle::gui_draw_rect(rect);
}

void Plugin::draw_circle(TGraphicCircle circle) {
    Kommunikationsstelle::gui_draw_circle(circle);
}

void Plugin::draw_line(TGraphicLine line) {
    Kommunikationsstelle::gui_draw_line(line);
}

void Plugin::draw_text(TGraphicText text) {
    Kommunikationsstelle::gui_draw_text(text);
}

void Plugin::draw_pixel(TGraphicPixel pixel) {
    Kommunikationsstelle::gui_draw_pixel(pixel);
}

void Plugin::clear() {
    Kommunikationsstelle::gui_clear();
}

void Plugin::on_error(int line, const std::string &message) {
    Kommunikationsstelle::on_error(line, message);
}

std::string Plugin::get_input_line(){
    return Kommunikationsstelle::get_input_line();
}
