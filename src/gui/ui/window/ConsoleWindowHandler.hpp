//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_CONSOLEWINDOWHANDLER_HPP
#define IMGUI_SETUP_CONSOLEWINDOWHANDLER_HPP

#include "base/WindowHandler.hpp"
#include "ConsoleWindow.hpp"


class ConsoleWindowHandler: public WindowHandler {
public:
    ConsoleWindowHandler(ConsoleWindow* window);
    void print(std::string message);
    void on_submit(console_submit_function callback);
private:
    ConsoleWindow* window;
};


#endif //IMGUI_SETUP_CONSOLEWINDOWHANDLER_HPP
