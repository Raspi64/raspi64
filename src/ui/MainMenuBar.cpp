//
// Created by Alex on 15.11.20.
//

#include "imgui.h"
#include "MainMenuBar.hpp"

void MainMenuBar::render() {
    if (ImGui::BeginMainMenuBar()) {
        ImGui::Text("HELP - Hilfe anzeigen ");
        ImGui::Text("CRTL + C - Programm beenden ");
        ImGui::Text("RUN - Programm starten ");
        ImGui::EndMainMenuBar();
    }
}
