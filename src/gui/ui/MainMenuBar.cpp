//
// Created by Alex on 15.11.20.
//

#include "imgui.h"
#include "MainMenuBar.hpp"

MainMenuBar::MainMenuBar(LANG *currentLanguage) : current_language(currentLanguage) {}

void MainMenuBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("STOP - Programm beenden (Konsole) ");
        ImGui::Text("START - Programm starten (Konsole) ");

        ImGui::Text("                      ");

        ImGui::Text("Aktuelle Sprache:");
        ImGui::Text((*current_language) == LUA ? "Lua" : "Basic");

        ImGui::Text("                      ");

        ImGui::Text("[ F10 ] - Sprache wechseln   ");
        ImGui::Text("                 [ F12 ] Herunterfahren");

        ImGui::EndMainMenuBar();
    }
}
