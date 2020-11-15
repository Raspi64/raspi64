//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_WINDOW_HPP
#define IMGUI_SETUP_WINDOW_HPP

#include "../../UiElement.hpp"


class Window: public UiElement {
public:
    virtual void render() = 0;
};


#endif //IMGUI_SETUP_WINDOW_HPP
