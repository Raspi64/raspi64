
#include <cstdio>
#include <unistd.h>
#include <Gui.hpp>
#include <fstream>
#include <iostream>
#include <utility>
#include "Schnittstelle.hpp"

Gui *gui;
Schnittstelle *sc;

bool loadfile(const std::string &file_path, std::string **result) {
    std::ifstream file_load_stream(file_path);
    if (!file_load_stream.good()) {
        return false;
    }
    std::string str((std::istreambuf_iterator<char>(file_load_stream)), std::istreambuf_iterator<char>());
    *result = new std::string((std::istreambuf_iterator<char>(file_load_stream)), std::istreambuf_iterator<char>());
    return true;
}

void my_draw(int x, int y, int red, int green, int blue, int alpha, int size) {
    gui->graphic->add_pixel(x, y, red, green, blue, alpha, size);
//    std::cout << "GUI Draw!" << std::endl;
}

void my_clear() {
    gui->graphic->clear_pixels();
}

void my_print(std::string message) {
    gui->console->print(std::move(message));
}

int main() {
    gui = new Gui();
    gui->initialize();

    sc = new Schnittstelle(LUA,my_print,my_draw,my_clear);

    std::string *script;
    if (!loadfile("../saves/drawer.lua", &script)) {
        std::cerr << "Error loading demo code!" << std::endl;
        return -1;
    }
    sc->start_script(*script);
    // TODO: When implemented
    //gui->editor->set_text(*script);
    delete script;

    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }
    gui->destroy();
}

