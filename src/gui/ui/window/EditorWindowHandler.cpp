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
