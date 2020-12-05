//
// Created by Alex on 15.11.20.
//

#include "EditorWindowHandler.hpp"
#include "base/WindowHandler.hpp"


EditorWindowHandler::EditorWindowHandler(EditorWindow *window): WindowHandler(window), window(window) {}

std::string EditorWindowHandler::get_text() {
    return window->get_text();
}

void EditorWindowHandler::set_text(const std::string& text) {
    window->set_text(text);
}

void EditorWindowHandler::set_error_marker(int linenum, std::string errormsg) {
    window->set_error_marker(linenum, errormsg);
}

void EditorWindowHandler::clear_error_markers() {
    window->clear_error_markers();
}
