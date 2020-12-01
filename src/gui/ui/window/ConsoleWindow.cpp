//
// Created by Alex on 15.11.20.
//

#include "imgui.h"

#include "ConsoleWindow.hpp"
#include "../../types.hpp"
#include "gui/config.hpp"


// Portable helpers
static int   Stricmp(const char* s1, const char* s2)         { int d; while ((d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; } return d; }
static int   Strnicmp(const char* s1, const char* s2, int n) { int d = 0; while (n > 0 && (d = toupper(*s2) - toupper(*s1)) == 0 && *s1) { s1++; s2++; n--; } return d; }
static char* Strdup(const char* s)                           { size_t len = strlen(s) + 1; void* buf = malloc(len); IM_ASSERT(buf); return (char*)memcpy(buf, (const void*)s, len); }
static void  Strtrim(char* s)                                { char* str_end = s + strlen(s); while (str_end > s && str_end[-1] == ' ') str_end--; *str_end = 0; }


ConsoleWindow::ConsoleWindow(): Window() {
    clear_log();
    memset(input_buf, 0, sizeof(input_buf));
    historyPos = -1;

    // "CLASSIFY" is here to provide the test case where "C"+[tab] completes to "CL" and display multiple matches.
    commands.push_back("HELP");
    commands.push_back("HISTORY");
    commands.push_back("CLEAR");
    commands.push_back("CLASSIFY");
    commands.push_back("RUN");
    commands.push_back("STOP");
    autoScroll = true;
    scrollToBottom = false;
    add_log("Welcome to Dear ImGui!");
}

ConsoleWindow::~ConsoleWindow(){
    clear_log();
    for (int i = 0; i < history.Size; i++)
        free(history[i]);
}

void ConsoleWindow::clear_log() {
    for (int i = 0; i < items.Size; i++)
        free(items[i]);
    items.clear();
}

void ConsoleWindow::add_log(const char *msg, ...) {
    char buf[1024];
    va_list args;
    va_start(args, msg);
    vsnprintf(buf, IM_ARRAYSIZE(buf), msg, args);
    buf[IM_ARRAYSIZE(buf)-1] = 0;
    va_end(args);
    items.push_back(Strdup(buf));
}


void ConsoleWindow::edit_callback(ImGuiInputTextCallbackData* data)
{
    //AddLog("cursor: %d, selection: %d-%d", data->CursorPos, data->SelectionStart, data->SelectionEnd);
    switch (data->EventFlag)
    {
        case ImGuiInputTextFlags_CallbackCompletion:
        {
            // Example of TEXT COMPLETION

            // Locate beginning of current word
            const char* word_end = data->Buf + data->CursorPos;
            const char* word_start = word_end;
            while (word_start > data->Buf)
            {
                const char c = word_start[-1];
                if (c == ' ' || c == '\t' || c == ',' || c == ';')
                    break;
                word_start--;
            }

            // Build a list of candidates
            ImVector<const char*> candidates;
            for (int i = 0; i < commands.Size; i++)
                if (Strnicmp(commands[i], word_start, (int)(word_end - word_start)) == 0)
                    candidates.push_back(commands[i]);

            if (candidates.Size == 0)
            {
                // No match
                add_log("No match for \"%.*s\"!\n", (int)(word_end - word_start), word_start);
            }
            else if (candidates.Size == 1)
            {
                // Single match. Delete the beginning of the word and replace it entirely so we've got nice casing.
                data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                data->InsertChars(data->CursorPos, candidates[0]);
                data->InsertChars(data->CursorPos, " ");
            }
            else
            {
                // Multiple matches. Complete as much as we can..
                // So inputing "C"+Tab will complete to "CL" then display "CLEAR" and "CLASSIFY" as matches.
                int match_len = (int)(word_end - word_start);
                for (;;)
                {
                    int c = 0;
                    bool all_candidates_matches = true;
                    for (int i = 0; i < candidates.Size && all_candidates_matches; i++)
                        if (i == 0)
                            c = toupper(candidates[i][match_len]);
                        else if (c == 0 || c != toupper(candidates[i][match_len]))
                            all_candidates_matches = false;
                    if (!all_candidates_matches)
                        break;
                    match_len++;
                }

                if (match_len > 0)
                {
                    data->DeleteChars((int)(word_start - data->Buf), (int)(word_end - word_start));
                    data->InsertChars(data->CursorPos, candidates[0], candidates[0] + match_len);
                }

                // List matches
                add_log("Possible matches:\n");
                for (int i = 0; i < candidates.Size; i++)
                    add_log("- %s\n", candidates[i]);
            }

            break;
        }
        case ImGuiInputTextFlags_CallbackHistory:
        {
            // Example of HISTORY
            const int prev_history_pos = historyPos;
            if (data->EventKey == ImGuiKey_UpArrow)
            {
                if (historyPos == -1)
                    historyPos = history.Size - 1;
                else if (historyPos > 0)
                    historyPos--;
            }
            else if (data->EventKey == ImGuiKey_DownArrow)
            {
                if (historyPos != -1)
                    if (++historyPos >= history.Size)
                        historyPos = -1;
            }

            // A better implementation would preserve the data on the current input line along with cursor position.
            if (prev_history_pos != historyPos)
            {
                const char* history_str = (historyPos >= 0) ? history[historyPos] : "";
                data->DeleteChars(0, data->BufTextLen);
                data->InsertChars(0, history_str);
            }
        }
    }
}

void ConsoleWindow::exec_command(const char* command_line)
{
    add_log("# %s\n", command_line);

    // Insert into history. First find match and delete it so it can be pushed to the back.
    // This isn't trying to be smart or optimal.
    historyPos = -1;
    for (int i = history.Size - 1; i >= 0; i--)
        if (Stricmp(history[i], command_line) == 0)
        {
            free(history[i]);
            history.erase(history.begin() + i);
            break;
        }
    history.push_back(Strdup(command_line));

    // Process command
    if (Stricmp(command_line, "CLEAR") == 0)
    {
        clear_log();
    }
    else if (Stricmp(command_line, "HELP") == 0)
    {
        add_log("Commands:");
        for (int i = 0; i < commands.Size; i++)
            add_log("- %s", commands[i]);
    }
    else if (Stricmp(command_line, "HISTORY") == 0)
    {
        int first = history.Size - 10;
        for (int i = first > 0 ? first : 0; i < history.Size; i++)
            add_log("%3d: %s\n", i, history[i]);
    }
    else
    {
        bool accepted = on_submit_fn(command_line);
        if (!accepted) {
            add_log("Unknown command: '%s'\n", command_line);
        }
    }

    // On command input, we scroll to bottom even if AutoScroll==false
    scrollToBottom = true;
}


void ConsoleWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(820, 570), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(1030, 430), ImGuiCond_None);

    ImGui::Begin(WIN_TITLE_CONSOLE, NULL, FLAGS_CONSOLE);
    after_imgui_begin(WIN_TITLE_CONSOLE);


    /*
    bool copy_to_clipboard = ImGui::SmallButton("Copy");

    ImGui::Separator();

    // Options menu
    if (ImGui::BeginPopup("Options"))
    {
        ImGui::Checkbox("Auto-scroll", &autoScroll);
        ImGui::EndPopup();
    }

    // Options, Filter
    if (ImGui::Button("Options"))
        ImGui::OpenPopup("Options");
    ImGui::SameLine();
    filter.Draw("Filter (\"incl,-excl\") (\"error\")", 180);
    ImGui::Separator();
    */

    // Reserve enough left-over height for 1 separator + 1 input text
    const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
    ImGui::BeginChild("ScrollingRegion", ImVec2(0, -footer_height_to_reserve), false, ImGuiWindowFlags_HorizontalScrollbar);
    if (ImGui::BeginPopupContextWindow())
    {
        if (ImGui::Selectable("Clear")) clear_log();
        ImGui::EndPopup();
    }


    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(4, 1)); // Tighten spacing
    /*
    if (copy_to_clipboard)
        ImGui::LogToClipboard();
    */
    for (int i = 0; i < items.Size; i++)
    {
        const char* item = items[i];
        if (!filter.PassFilter(item))
            continue;

        // Normally you would store more information in your item than just a string.
        // (e.g. make Items[] an array of structure, store color/type etc.)
        ImVec4 color;
        bool has_color = false;
        if (strstr(item, "[error]"))          { color = ImVec4(1.0f, 0.4f, 0.4f, 1.0f); has_color = true; }
        else if (strncmp(item, "# ", 2) == 0) { color = ImVec4(1.0f, 0.8f, 0.6f, 1.0f); has_color = true; }
        if (has_color)
            ImGui::PushStyleColor(ImGuiCol_Text, color);
        ImGui::TextUnformatted(item);
        if (has_color)
            ImGui::PopStyleColor();
    }
    /*
    if (copy_to_clipboard)
        ImGui::LogFinish();
    */

    if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY()))
        ImGui::SetScrollHereY(1.0f);
    scrollToBottom = false;

    ImGui::PopStyleVar();
    ImGui::EndChild();
    ImGui::Separator();

    // Command-line
    bool reclaim_focus = false;
    ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
    ImGui::PushItemWidth(-1);
    //if (ImGui::InputText("##consoleInput", input_buf, IM_ARRAYSIZE(input_buf), input_text_flags, &TextEditCallbackStub, (void*)this))


    if (ImGui::IsWindowFocused() && !ImGui::IsAnyItemActive())
        ImGui::SetKeyboardFocusHere(0);

    if (ImGui::InputText("##consoleInput", input_buf, IM_ARRAYSIZE(input_buf), input_text_flags, [](ImGuiInputTextCallbackData* data) {
        ConsoleWindow *console = (ConsoleWindow *) data->UserData;
        console->edit_callback(data);
        return 0;
    }, (void*)this)) {
        char* s = input_buf;
        Strtrim(s);
        if (s[0])
            exec_command(s);
        strcpy(s, "");
        reclaim_focus = true;
    }

    ImGui::PopItemWidth();

    // Auto-focus on window apparition
    ImGui::SetItemDefaultFocus();
    if (reclaim_focus)
        ImGui::SetKeyboardFocusHere(-1); // Auto focus previous widget

    ImGui::End();
}

void ConsoleWindow::on_submit(console_submit_function callback) {
    on_submit_fn = callback;
}
