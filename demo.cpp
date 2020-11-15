//
// Created by Alex on 15.11.20.
//

#include "src/Gui.hpp"

Gui* gui;

int count = 100;

void initialize()
{
    gui = new Gui();
    gui->initialize();

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
}

int tick()
{
    count --;

    if (count == 0) {
        // hide the smiley after 100 ticks
        gui->graphic->clear_pixels();
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




