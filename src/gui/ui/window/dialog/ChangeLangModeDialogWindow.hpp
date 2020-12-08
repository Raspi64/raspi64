//
// Created by Alex on 05.12.20.
//

#ifndef IMGUI_SETUP_CHANGELANGMODEDIALOGWINDOW_HPP
#define IMGUI_SETUP_CHANGELANGMODEDIALOGWINDOW_HPP

#include <typeinfo>
#include "base/Window.hpp"
#include "types.hpp"


class ChangeLangModeDialogWindow: public Window {
public:
    ChangeLangModeDialogWindow(
            std::function<void(LANG newLang)> callback,
            LANG* current_language);
    void render();

private:
    std::function<void(LANG newLang)> callback;
    LANG* current_language;

    void trigger_change();
};


#endif //IMGUI_SETUP_CHANGELANGMODEDIALOGWINDOW_HPP
