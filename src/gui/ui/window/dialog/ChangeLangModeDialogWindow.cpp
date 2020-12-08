//
// Created by Alex on 05.12.20.
//

#include <stdlib.h>
#include "imgui.h"

#include "ChangeLangModeDialogWindow.hpp"
#include "base/Window.hpp"
#include "gui/config.hpp"


ChangeLangModeDialogWindow::ChangeLangModeDialogWindow(
        std::function<void(LANG newLang)> callback,
        LANG* current_language
        ): Window(), callback(callback), current_language(current_language) {
}

void ChangeLangModeDialogWindow::render() {
// Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(WIN_TITLE_CHANGELANGMODE, NULL,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("\nSoll die Programmiersprache gewechselt werden?\n\n");
        ImGui::Text("Die neue Sprache ist:");
        ImGui::SameLine();
        ImGui::Text((*current_language) == LUA ? "Basic" : "Lua");
        ImGui::Text("\n\n\n");
        ImGui::Separator();


        if (ImGui::Button("Abbrechen\n (ESC)", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        // Button Abbruch is activated
        if (ImGui::IsKeyPressed(KEY_ESC)) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("Wechseln\n(Enter)", ImVec2(120, 0))) {
            trigger_change();
        }
        // Button Ok is activated
        if (ImGui::IsKeyPressed(KEY_ENTER)) {
            trigger_change();
        }
        ImGui::EndPopup();
    }
}

void ChangeLangModeDialogWindow::trigger_change() {
    callback(*current_language == LUA ? BASIC : LUA);
    ImGui::CloseCurrentPopup();
}
