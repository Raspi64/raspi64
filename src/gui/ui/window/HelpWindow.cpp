//
// Created by Alex on 15.11.20.
//

#include "imgui.h"

#include "HelpWindow.hpp"
#include "gui/config.hpp"

HelpWindow::HelpWindow(LANG initialLanguage)
        : Window(),
        root_common(nullptr),
        root_language(nullptr),
        mode_search(false),
        search_results(),
        on_request_common_root_fn(nullptr),
        on_request_language_root_fn(nullptr),
        on_search_fn(nullptr) {
    set_language_mode(initialLanguage);
}

void HelpWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(820, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);

    //Help window
    ImGui::Begin(WIN_TITLE_HELP, NULL, FLAGS_HELP);
    after_imgui_begin(WIN_TITLE_HELP);

    if (ImGui::IsWindowFocused() && ImGui::IsKeyPressed(KEY_F1)) {
        this->clear_search();
    }

    ImGui::Text("Wenn Hilfefenster Fokus hat:");
    ImGui::TextWrapped("Drücke ESC, dann Pfeiltasten, dann LEER um zwischen der Suchzeile und dem Themenbaum zu wechseln");
    ImGui::Separator();

    ImGui::Text("Navigation im Themenbaum:");
    ImGui::Text("LINKS/RECHTS: Auf-/Einklappen\nHOCH/RUNTER: Bewegen");
    ImGui::Separator();

    static char buf[32] = "";

    if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive() && !ImGui::IsAnyItemFocused())
        ImGui::SetKeyboardFocusHere(0);

    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue;

    if (this->on_search_fn != nullptr) {
        if (ImGui::Button("Start [F1]")) {
            this->clear_search();
        }

        ImGui::SameLine();
        ImGui::Text("|");

        ImGui::SameLine();
        if (ImGui::InputTextWithHint("", "Suchen", buf, IM_ARRAYSIZE(buf), input_text_flags)) {
            char *s = buf;
            if (s[0]) {
                this->search_results = this->on_search_fn(s);
                this->mode_search = true;
            } else {
                this->mode_search = false;
            }
        }
        ImGui::SetItemDefaultFocus();

        ImGui::SameLine();
        ImGui::Text("[Enter]");
    }

    ImGui::Separator();

    // Begin entries
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

    if (this->mode_search) {
        if (this->search_results.empty()) {
            ImGui::Text("Keine Ergebnisse.");
        } else {
            for (auto &entry : this->search_results) {
                this->process_entry(entry);
            }
        }
    } else {
        // render help structure
        if (this->root_common != nullptr) {
            this->process_entry(this->root_common);
        }
        if (this->root_language != nullptr) {
            this->process_entry(this->root_language);
        }
    }

    // end entries
    ImGui::EndChild();

    ImGui::End();
}

/*
void HelpWindow::searchinput_callback(ImGuiInputTextCallbackData* data) {
    switch (data->EventFlag)
    {

    }
}
*/

void HelpWindow::process_entry(Entry *entry) {
    if (ImGui::TreeNode(entry->name.c_str())) {
        if (entry->sub_entries.size()) {
            for (size_t i = 0; i < entry->sub_entries.size(); i++) {
                this->process_entry(&entry->sub_entries[i]);
            }
        } else {
            // show help content
            ImGui::TextWrapped(entry->content.c_str());
            ImGui::Text("\n\n");
        }

        ImGui::TreePop();
    }
}

void HelpWindow::clear_search() {
    this->mode_search = false;
    this->search_results.clear();
}

void HelpWindow::set_language_mode(LANG lang) {
    this->current_language = lang;
    if (this->on_request_common_root_fn != nullptr) {
        this->root_common = this->on_request_common_root_fn();
    }
    if (this->on_request_language_root_fn != nullptr) {
        this->root_language = this->on_request_language_root_fn();
    }
    printf("");
}

void HelpWindow::on_request_common_root(helpentryroot_funct_t function) {
    this->on_request_common_root_fn = function;
    this->set_language_mode(this->current_language);
}

void HelpWindow::on_request_language_root(helpentryroot_funct_t function) {
    this->on_request_language_root_fn = function;
    this->set_language_mode(this->current_language);
}

void HelpWindow::on_search(helpentries_search_funct_t function) {
    this->on_search_fn = function;
}
