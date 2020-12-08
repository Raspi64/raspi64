//
// Created by Alex on 15.11.20.
//

#include "imgui.h"
#include "MainMenuBar.hpp"

MainMenuBar::MainMenuBar(LANG *currentLanguage) : current_language(currentLanguage) {}

void MainMenuBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("HELP - Hilfe anzeigen ");
        ImGui::Text("CRTL + C - Programm beenden ");
        ImGui::Text("RUN - Programm starten ");

        ImGui::Text("Aktuelle Sprache:");
        ImGui::Text((*current_language) == LUA ? "Lua" : "Basic");

        ImGui::Text("                 [ F12 ] Herunterfahren");

        ImGui::EndMainMenuBar();
    }
}
