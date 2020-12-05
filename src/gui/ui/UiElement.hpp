//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_UIELEMENT_HPP
#define IMGUI_SETUP_UIELEMENT_HPP


class UiElement {
public:
    UiElement();

    virtual void render() = 0;
};


#endif //IMGUI_SETUP_UIELEMENT_HPP
