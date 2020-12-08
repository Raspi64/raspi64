//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_HELPWINDOW_HPP
#define IMGUI_SETUP_HELPWINDOW_HPP

#include <types.hpp>
#include "base/Window.hpp"


class HelpWindow: public Window {
public:
    HelpWindow(LANG initialLanguage);
    void render();

    void set_language_mode(LANG lang);
private:
    LANG current_language;
};


#endif //IMGUI_SETUP_HELPWINDOW_HPP
