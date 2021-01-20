#include <Gui.hpp>
#include <iostream>
#include "Kommunikationsstelle.hpp"

int main() {
    // Initialize Gui
    Gui *gui = new Gui();
    gui->initialize();

    // Initialize Schnittstelle
    Kommunikationsstelle::init(gui, LUA);

    // Register Bindings
    gui->on_change_langmode_request(Kommunikationsstelle::set_language);
    gui->on_keydown(Kommunikationsstelle::on_key_press);
    gui->on_keyup(Kommunikationsstelle::on_key_release);
    gui->console->on_submit(Kommunikationsstelle::handle_command);
    gui->help->on_request_common_root(Kommunikationsstelle::get_common_help_root);
    gui->help->on_request_language_root(Kommunikationsstelle::get_language_help_root);
    gui->help->on_search(Kommunikationsstelle::search_entries);

    // Load example Program
    const std::string &script = Kommunikationsstelle::load("example_colors_rainbow");
    gui->editor->set_text(script);

    // Run example Program
    Kommunikationsstelle::start_script(script);

    // Tick the Gui until User wants to exit
    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }

    // De-Initialize Gui
    gui->destroy();
}
