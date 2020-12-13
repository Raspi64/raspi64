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

void my_change_language(LANG lang) {
    sc->set_language(lang);
    gui->set_language_mode(lang);
}

bool my_on_submit(std::string command) {
    if (command == "start" || command == "run") {
        sc->start_script(gui->editor->get_text());
        return true;
    }
    if (command == "stop") {
        sc->kill_current_task();
        return true;
    }
    if (command.find("save") == 0 || command.find("load") == 0) {
        unsigned long pos = command.find(' ');
        if (pos == std::string::npos) {
            gui->console->print("[error] Kein Dateinamen gefunden");
            return true;
        }
        std::string name = command.substr(pos + 1);
        if (name.find(' ') != std::string::npos) {
            gui->console->print("[error] Dateiname darf keine Leerzeichen enthalten");
            return true;
        }
        if (name.find('/') != std::string::npos) {
            gui->console->print("[error] Dateiname darf keine Slashes enthalten");
            return true;
        }
        if (command[0] == 's') {
            sc->save(name, gui->editor->get_text());
            return true;
        } else {
            gui->editor->set_text(sc->load(name));
            return true;
        }
    }
    return false;
}

void my_keydown(const SDL_Keysym keysym) {
    std::cout << "key: " << keysym.scancode << std::endl;
}

void on_error(int line, const std::string &message) {
    gui->console->print("[error] " + message);
    gui->editor->set_error_marker(line, message);
}

void clear_error() {
    gui->editor->clear_error_markers();
}

Entry *get_common_help_root() {
    return sc->get_common_help_root();
}

Entry *get_language_help_root() {
    return sc->get_language_help_root();
}

std::vector<Entry *> search_entries(const std::string &searchword) {
    return sc->search_entries(searchword);
}

void run_through_entries(Entry *parent, unsigned int depth) {
    for (unsigned int i = 0; i < depth; ++i) {
        std::cout << ' ';
    }

    std::cout << parent->name;
    if (parent->is_file) {
        std::cout << std::endl;
    } else {
        std::cout << "/" << std::endl;
        for (auto &entry : parent->sub_entries) {
            run_through_entries(&entry, depth + 2);
        }
    }
}

int main() {
    gui = new Gui();
    gui->initialize();

    sc = new Schnittstelle(BASIC, my_print, my_draw, my_clear);

//    const std::vector<Entry *> &vector = search_entries("print");
//    for (auto &entry : vector) {
//        std::cout << entry->name << std::endl;
//    }

    std::cout << "Help files:" << std::endl;
    run_through_entries(sc->get_common_help_root(), 2);
    run_through_entries(sc->get_language_help_root(), 2);
    sc->set_language(LUA);
    run_through_entries(sc->get_language_help_root(), 2);

    //return 0;

    gui->help->on_request_common_root(get_common_help_root);
    gui->help->on_request_language_root(get_language_help_root);
    gui->help->on_search(search_entries);

    gui->on_change_langmode_request(my_change_language);
    gui->console->on_submit(my_on_submit);
    gui->on_keydown(my_keydown);

    std::string script;
    if (!loadfile("../saves/drawer.lua", &script)) {
        std::cerr << "Error loading demo code!" << std::endl;
        return -1;
    }

    gui->editor->set_text(script);

    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }
    gui->destroy();
}

