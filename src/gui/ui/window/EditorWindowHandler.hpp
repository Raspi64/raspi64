//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_EDITORWINDOWHANDLER_HPP
#define IMGUI_SETUP_EDITORWINDOWHANDLER_HPP

#include "base/WindowHandler.hpp"
#include "EditorWindow.hpp"


class EditorWindowHandler: public WindowHandler {
public:
    explicit EditorWindowHandler(EditorWindow* window);
    std::string get_text();
    void set_text(const std::string&);
    void set_error_marker(int linenum, std::string errormsg);
    void clear_error_markers();
private:
    EditorWindow* window;
};


#endif //IMGUI_SETUP_EDITORWINDOWHANDLER_HPP
