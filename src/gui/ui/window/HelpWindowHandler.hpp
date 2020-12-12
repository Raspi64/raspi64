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

    void on_request_common_root(helpentryroot_funct_t function);
    void on_request_language_root(helpentryroot_funct_t function);
    void on_search(helpentries_search_funct_t function);
private:
    HelpWindow* window;
};


#endif //IMGUI_SETUP_HELPWINDOWHANDLER_HPP
