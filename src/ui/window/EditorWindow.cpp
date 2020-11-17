//
// Created by Alex on 15.11.20.
//

#include <stdio.h>
#include "imgui.h"

#include "EditorWindow.hpp"
#include "EditorWindowHandler.hpp"
#include "../../config.hpp"


EditorWindow::EditorWindow(): Window() {}

void EditorWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(60, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(700, 950), ImGuiCond_None);

    ImGui::Begin(WIN_TITLE_EDITOR, NULL, FLAGS_EDITOR);                          // Create a window called "Hello, world!" and append into it.

    static char text[1024 * 16] =
            "10 PRINT HELLO WORLD\n"
            "20 START\n"
            "30 RESET";
    static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;

    if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
        ImGui::SetKeyboardFocusHere(0);

    ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, 915), flags);
    ImGui::SetItemDefaultFocus();

    ImGui::End();
}
