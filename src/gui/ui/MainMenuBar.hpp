//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_MAINMENUBAR_HPP
#define IMGUI_SETUP_MAINMENUBAR_HPP

#include <types.hpp>
#include "UiElement.hpp"

class MainMenuBar: public UiElement {
public:
    MainMenuBar(LANG* current_language);

    void render();

private:
    LANG* current_language;
};

#endif //IMGUI_SETUP_MAINMENUBAR_HPP
