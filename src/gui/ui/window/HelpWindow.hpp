//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_HELPWINDOW_HPP
#define IMGUI_SETUP_HELPWINDOW_HPP

#include <types.hpp>
#include "base/Window.hpp"


class HelpWindow: public Window {
public:
    HelpWindow(LANG initialLanguage);
    void render();

    void set_language_mode(LANG lang);
    void on_request_common_root(helpentryroot_funct_t function);
    void on_request_language_root(helpentryroot_funct_t function);
    void on_search(helpentries_search_funct_t function);
private:
    LANG current_language;
    Entry* root_common;
    Entry* root_language;
    bool mode_search;
    std::vector<Entry*> search_results;

    void process_entry(Entry* entry);
    //void searchinput_callback(ImGuiInputTextCallbackData* data);
    void clear_search();

    helpentryroot_funct_t on_request_common_root_fn;
    helpentryroot_funct_t on_request_language_root_fn;
    helpentries_search_funct_t on_search_fn;
};


#endif //IMGUI_SETUP_HELPWINDOW_HPP
