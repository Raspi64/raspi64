//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_WINDOW_HPP
#define IMGUI_SETUP_WINDOW_HPP

#include <string>

#include "../../UiElement.hpp"


class Window: public UiElement {
public:
    Window();
    virtual void render() = 0;
    void after_imgui_begin(std::string window_title);
    static std::string current_window;
};


#endif //IMGUI_SETUP_WINDOW_HPP
