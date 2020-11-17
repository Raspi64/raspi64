//
// Created by Alex on 15.11.20.
//

#include <stdio.h>
#include <stdlib.h>
#include "imgui.h"

#include "ShutdownDialogWindow.hpp"
#include "base/Window.hpp"
#include "../../config.hpp"


ShutdownDialogWindow::ShutdownDialogWindow() : Window() {}

void ShutdownDialogWindow::render() {
    // Always center this window when appearing
    ImVec2 center(ImGui::GetIO().DisplaySize.x * 0.5f, ImGui::GetIO().DisplaySize.y * 0.5f);
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));

    if (ImGui::BeginPopupModal(WIN_TITLE_SHUTDOWN, NULL,
                               ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoMove)) {
        ImGui::Text("\nMöchten Sie das System wirklich herunterfahren?\n\n\n\n");
        ImGui::Separator();


        if (ImGui::Button("Abbruch\n (ESC)", ImVec2(120, 0))) {
            ImGui::CloseCurrentPopup();
        }
        // Button Abbruch is activated
        if (ImGui::IsKeyPressed(KEY_ESC)) {
            ImGui::CloseCurrentPopup();
        }

        ImGui::SetItemDefaultFocus();
        ImGui::SameLine();

        if (ImGui::Button("  OK\n(Enter)", ImVec2(120, 0))) {
            system("shutdown -P now");
        }
        // Button Ok is activated
        if (ImGui::IsKeyPressed(KEY_ENTER)) {
            //sync();
            //reboot(LINUX_REBOOT_CMD_POWER_OFF);
            system("shutdown -P now");

        }
        ImGui::EndPopup();
    }
}
