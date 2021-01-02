//
// Created by simon on 12/1/20.
//

#include "Plugin.hpp"
#include "Schnittstelle.hpp"

void Plugin::print(const std::string &message) {
    Schnittstelle::gui_print(message);
}

void Plugin::draw(int x, int y, int red, int green, int blue, int alpha, int size) {
    Schnittstelle::gui_draw(x, y, red, green, blue, alpha, size);
}

void Plugin::clear() {
    Schnittstelle::gui_clear();
}

void Plugin::on_error(int line, const std::string &message) {
    Schnittstelle::on_error(line, message);
}

bool Plugin::waiting_for_input;
std::string Plugin::input;
bool Plugin::input_ready;
