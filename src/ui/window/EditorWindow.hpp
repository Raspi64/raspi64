//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_EDITORWINDOW_HPP
#define IMGUI_SETUP_EDITORWINDOW_HPP

#include "base/Window.hpp"
#include <TextEditor.h>


class EditorWindow : public Window {
public:
    EditorWindow();

    bool open_file(const std::string& path);

    void render() override;

    std::string get_text();
private:
    TextEditor editor;
    std::string openFilePath;
};


#endif //IMGUI_SETUP_EDITORWINDOW_HPP
