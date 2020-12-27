#include <Gui.hpp>
#include <fstream>
#include <iostream>
#include "Schnittstelle.hpp"

Gui *gui;

void my_keydown(const SDL_Keysym keysym) {
    std::cout << "key: " << keysym.scancode << std::endl;
}

void run_through_entries(Entry *parent, unsigned int indentation);

int main() {
    gui = new Gui();
    gui->initialize();

    Schnittstelle::init(gui, BASIC);

//    const std::vector<Entry *> &vector = search_entries("print");
//    for (auto &entry : vector) {
//        std::cout << entry->name << std::endl;
//    }

    std::cout << "Help files:" << std::endl;
    run_through_entries(Schnittstelle::get_common_help_root(), 2);
    run_through_entries(Schnittstelle::get_language_help_root(), 2);
    Schnittstelle::set_language(LUA);
    run_through_entries(Schnittstelle::get_language_help_root(), 2);

    //return 0;

    gui->help->on_request_common_root(Schnittstelle::get_common_help_root);
    gui->help->on_request_language_root(Schnittstelle::get_language_help_root);
    gui->help->on_search(Schnittstelle::search_entries);

    gui->on_change_langmode_request(Schnittstelle::set_language);
    gui->console->on_submit(Schnittstelle::handle_command);
    gui->on_keydown(my_keydown);

    Schnittstelle::load("drawer");

    Schnittstelle::start_script(gui->editor->get_text());

    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }
    gui->destroy();
}

void run_through_entries(Entry *parent, unsigned int indentation) {
    for (unsigned int i = 0; i < indentation; ++i) {
        std::cout << ' ';
    }

    std::cout << parent->name;
    if (parent->is_file) {
        std::cout << std::endl;
    } else {
        std::cout << "/" << std::endl;
        for (auto &entry : parent->sub_entries) {
            run_through_entries(&entry, indentation + 2);
        }
    }
}
