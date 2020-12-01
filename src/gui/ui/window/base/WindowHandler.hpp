//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_WINDOWHANDLER_HPP
#define IMGUI_SETUP_WINDOWHANDLER_HPP

#include "Window.hpp"


class WindowHandler {
public:
    WindowHandler(Window* window);
private:
    Window* window;
};


#endif //IMGUI_SETUP_WINDOWHANDLER_HPP
