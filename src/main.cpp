#include <Gui.hpp>
#include <iostream>
#include "Schnittstelle.hpp"

int main() {
    // Initialize Gui
    Gui* gui = new Gui();
    gui->initialize();

    // Initialize Schnittstelle
    Schnittstelle::init(gui, LUA);

    // Register Bindings
    gui->on_change_langmode_request(Schnittstelle::set_language);
    gui->on_keydown(Schnittstelle::on_key_press);
    gui->on_keyup(Schnittstelle::on_key_release);
    gui->console->on_submit(Schnittstelle::handle_command);
    gui->help->on_request_common_root(Schnittstelle::get_common_help_root);
    gui->help->on_request_language_root(Schnittstelle::get_language_help_root);
    gui->help->on_search(Schnittstelle::search_entries);

    // Load example Program
    const std::string &script = Schnittstelle::load("example_colors_rainbow");
    gui->editor->set_text(script);

    // Run example Program
    Schnittstelle::start_script(script);

    // Tick the Gui until User wants to exit
    bool running = true;
    while (running) {
        running = gui->tick() == 0;
    }

    // De-Initialize Gui
    gui->destroy();
}
