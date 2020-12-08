//
// Created by Alex on 15.11.20.
//

#include <cstdio>
#include <fstream>
#include <utility>
#include <imgui.h>

#include <EditorWindow.hpp>
#include <EditorWindowHandler.hpp>
#include <gui/config.hpp>
#include "../../extension/TextEditorLanguageExtension.hpp"


EditorWindow::EditorWindow(LANG initialLanguage) : Window() {
    set_language_mode(initialLanguage);
}

bool EditorWindow::open_file(const std::string &path) {
    openFilePath = path;

    std::ifstream stream(path);
    if (stream.good()) {
        std::string str((std::istreambuf_iterator<char>(stream)), std::istreambuf_iterator<char>());
        editor.SetText(str);
        return true;
    }
    return false;
}

void EditorWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(60, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(700, 950), ImGuiCond_None);

    auto cpos = editor.GetCursorPosition();
    ImGui::Begin(WIN_TITLE_EDITOR, nullptr, FLAGS_EDITOR);
    after_imgui_begin(WIN_TITLE_EDITOR);

    ImGui::SetWindowSize(ImVec2(800, 600), ImGuiCond_FirstUseEver);
    if (ImGui::BeginMenuBar()) {
        if (ImGui::BeginMenu("Datei")) {
            if (ImGui::MenuItem("Speichern")) {
                auto textToSave = editor.GetText();
                /// save text....
                printf("Speichert: \"%s\n\"", textToSave.c_str());
            }
            ImGui::EndMenu();
        }

        ImGui::EndMenuBar();
    }

    ImGui::Text(
            "%6d/%-6d %6d Zeilen  | %s | %s | %s | %s",
            cpos.mLine + 1,
            cpos.mColumn + 1,
            editor.GetTotalLines(),
            editor.IsOverwrite() ? "Ovr" : "Ins",
            editor.CanUndo() ? "*" : " ",
            editor.GetLanguageDefinition().mName.c_str(),
            openFilePath.c_str()
    );
    ImGui::SetItemDefaultFocus();

    if (ImGui::IsWindowFocused())
        ImGui::SetNextWindowFocus();
    editor.Render("TextEditor");

    ImGui::End();
}

std::string EditorWindow::get_text() {
    return editor.GetText();
}

void EditorWindow::set_text(const std::string& text) {
    editor.SetText(text);
}

void EditorWindow::set_error_marker(int linenum, std::string errormsg) {
    TextEditor::ErrorMarkers markers;
    markers.insert(std::make_pair(linenum, errormsg));
    editor.SetErrorMarkers(markers);
}

void EditorWindow::clear_error_markers() {
    TextEditor::ErrorMarkers markers;
    editor.SetErrorMarkers(markers);
}

void EditorWindow::set_language_mode(LANG lang) {
    const TextEditor::LanguageDefinition & definition = lang == LUA
            ? TextEditor::LanguageDefinition::Lua()
            : LanguageDefinitionExtension::Basic();

    editor.SetLanguageDefinition(definition);
}
