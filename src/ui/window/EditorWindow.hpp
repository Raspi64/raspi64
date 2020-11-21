//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_EDITORWINDOW_HPP
#define IMGUI_SETUP_EDITORWINDOW_HPP

#include "base/Window.hpp"


class EditorWindow: public Window {
public:
    EditorWindow();
    void render();
    char* get_text();
};


#endif //IMGUI_SETUP_EDITORWINDOW_HPP
