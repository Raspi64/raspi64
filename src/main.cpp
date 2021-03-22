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
    unsigned int frame = 0;
    unsigned int frame_sec = 0;
    struct timespec now, later;

    while (running) {
        printf("Rendering frame % 5d ", frame);
        clock_gettime(CLOCK_REALTIME, &now);

        running = gui->tick() == 0;
        frame++;
        frame_sec++;

        clock_gettime(CLOCK_REALTIME, &later);
        __syscall_slong_t diff_ns = later.tv_nsec - now.tv_nsec;
        printf(" took % 3ld.%03ld.%03ld ps\n", diff_ns / 1000000, (diff_ns / 1000) % 1000, diff_ns % 1000);

        if (now.tv_sec != later.tv_sec) {
            printf("%d FPS\n", frame_sec);
            frame_sec = 0;
        }
    }

    // De-Initialize Gui
    gui->destroy();
}
