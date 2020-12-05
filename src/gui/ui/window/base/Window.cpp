//
// Created by Alex on 15.11.20.
//

#include <string>
#include "imgui.h"

#include "Window.hpp"


std::string Window::current_window;

Window::Window() : UiElement() {}

void Window::after_imgui_begin(std::string window_title) {
    if (ImGui::IsWindowFocused())
        Window::current_window = window_title;
}
