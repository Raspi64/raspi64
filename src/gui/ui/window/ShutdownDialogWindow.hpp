//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_SHUTDOWNDIALOGWINDOW_HPP
#define IMGUI_SETUP_SHUTDOWNDIALOGWINDOW_HPP

#include "base/Window.hpp"


class ShutdownDialogWindow: public Window {
public:
    ShutdownDialogWindow();
    void render();
};


#endif //IMGUI_SETUP_SHUTDOWNDIALOGWINDOW_HPP
