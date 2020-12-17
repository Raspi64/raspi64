#include <Gui.hpp>
#include <fstream>
#include <iostream>
#include "Schnittstelle.hpp"

Gui *gui;
Schnittstelle *sc;

bool loadfile(const std::string &file_path, std::string *result) {
    std::ifstream file_load_stream(file_path);
    if (!file_load_stream.good()) {
        return false;
    }
    std::string str((std::istreambuf_iterator<char>(file_load_stream)), std::istreambuf_iterator<char>());
    *result = str;
    return true;
}

void my_draw(int x, int y, int red, int green, int blue, int alpha, int size) {
    gui->graphic->add_pixel(x, y, red, green, blue, alpha, size);
//    std::cout << "GUI Draw!" << std::endl;
}

void my_clear() {
    gui->graphic->clear_pixels();
//    std::cout << "GUI Clear!" << std::endl;
}

void my_print(std::string message) {
    gui->console->print(message);
    std::cout << "Print: " << message << std::endl;
}

void my_change_language(LANG lang){
    sc->set_language(lang);
    gui->set_language_mode(lang);
}

bool my_on_submit(std::string command){
    if (command == "start" || command == "run") {
        sc->start_script(gui->editor->get_text());
        return true;
    }
    if (command == "stop"){
        sc->kill_current_task();
        return true;
    }
}

void my_keydown(const SDL_Keysym keysym){
    std::cout<<"key: " << keysym.scancode << std::endl;
}

int main() {
    gui = new Gui();
    gui->initialize();

    sc = new Schnittstelle(LUA, my_print, my_draw, my_clear);

    gui->on_change_langmode_request(my_change_language);
    gui->console->on_submit(my_on_submit);
    gui->on_keydown(my_keydown);

    std::string script;
    if (!loadfile("../saves/drawer.lua", &script)) {
        std::cerr << "Error loading demo code!" << std::endl;
        return -1;
    }

    //    sc = new Schnittstelle(BASIC, my_print, my_draw, my_clear);
    //
    //    std::string script = "PRINT \"Hello\"";
    //    if (!loadfile("../saves/drawer.lua", &script)) {
    //        std::cerr << "Error loading demo code!" << std::endl;
    //        return -1;
    //    }
    //    std::cout << "Script: \"" << script << '\"' << std::endl;
    //    std::cout << "Running script..." << std::endl;

    sc->start_script(script);
    gui->editor->set_text(script);

    gui->editor->set_error_marker(3, "Hallo");

    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }
    gui->destroy();
}

