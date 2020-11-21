//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_EDITORWINDOWHANDLER_HPP
#define IMGUI_SETUP_EDITORWINDOWHANDLER_HPP

#include "base/WindowHandler.hpp"
#include "EditorWindow.hpp"


class EditorWindowHandler: public WindowHandler {
public:
    EditorWindowHandler(EditorWindow* window);
    char* get_text();
private:
    EditorWindow* window;
};


#endif //IMGUI_SETUP_EDITORWINDOWHANDLER_HPP
