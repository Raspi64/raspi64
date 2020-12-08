//
// Created by Alex on 15.11.20.
//

#include <SDL.h>

#include "gui/Gui.hpp"


Gui* gui;

int smiley_cooldown = 500;


void key_down(const SDL_Keysym keysym)
{
    printf("key pressed: %s\n", SDL_GetKeyName(keysym.sym));
}

void langmode_change_request(LANG newLang)
{
    printf("langmode change request incoming");
    // change language

    gui->set_language_mode(newLang);
}

bool console_submit(std::string message)
{
    printf("console submit: %s\n", message.c_str());

    return (message == "will be accepted");
}

void initialize()
{
    gui = new Gui();
    gui->initialize();

    // register callbacks
    gui->on_keydown(key_down);
    gui->on_change_langmode_request(langmode_change_request);
    gui->console->on_submit(console_submit);

    // Draw a demo smiley
    gui->graphic->add_pixel(50, 50, 255, 0, 0, 255, 15);
    gui->graphic->add_pixel(200, 50, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(50, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(80, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(110, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(140, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(170, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(200, 150, 255, 0,0, 255, 15);
    gui->graphic->add_pixel(135, 75, 255, 0,0, 255, 15);

    gui->editor->set_text("Hallo\nWelt\nFoo\nBar\n1234");
    gui->editor->set_error_marker(3, "Hallo");
}

int tick()
{
    smiley_cooldown --;

    if (smiley_cooldown == 0) {
        // hide the smiley after 100 ticks
        gui->graphic->clear_pixels();
        printf("editor content %s", gui->editor->get_text().c_str());
        gui->console->print("this is a test message printed to the console");
        gui->editor->clear_error_markers();
    }

    return gui->tick();
}

void destroy()
{
    gui->destroy();
}




int main(int, char**)
{
    initialize();

    bool running = true;

    while(running) {
        running = tick() == 0;
    }

    destroy();
}




