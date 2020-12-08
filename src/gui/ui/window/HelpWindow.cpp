//
// Created by Alex on 15.11.20.
//

#include "imgui.h"

#include "HelpWindow.hpp"
#include "gui/config.hpp"

HelpWindow::HelpWindow(LANG initialLanguage): Window() {
    set_language_mode(initialLanguage);
}

void HelpWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(820, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);

    //Help window
    ImGui::Begin(WIN_TITLE_HELP, NULL, FLAGS_HELP);
    after_imgui_begin(WIN_TITLE_HELP);

    static char buf[32] = "Ägypten ÜÖÄ";

    if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
        ImGui::SetKeyboardFocusHere(0);
    ImGui::InputText("Search", buf, IM_ARRAYSIZE(buf));
    ImGui::SetItemDefaultFocus();

    ImGui::TextWrapped(
            "\nThis text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
            "This text is just a Test\n"
    );
    ImGui::End();
}

void HelpWindow::set_language_mode(LANG lang) {
    this->current_language = lang;
}
