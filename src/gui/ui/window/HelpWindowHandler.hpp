//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_HELPWINDOWHANDLER_HPP
#define IMGUI_SETUP_HELPWINDOWHANDLER_HPP

#include "base/WindowHandler.hpp"
#include "HelpWindow.hpp"


class HelpWindowHandler: public WindowHandler {
public:
    HelpWindowHandler(HelpWindow* window);
private:
    HelpWindow* window;
};


#endif //IMGUI_SETUP_HELPWINDOWHANDLER_HPP
