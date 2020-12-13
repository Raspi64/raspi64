//
// Created by simon on 12/1/20.
//

#include <iostream>
#include <fstream>
#include "BasicPlugin.hpp"
#include "LuaPlugin.hpp"
#include "Plugin.hpp"
#include "Schnittstelle.hpp"
#include "HelpSystem.hpp"


void *Schnittstelle::exec_script(void *params_void) {
    auto schnittstelle = (Schnittstelle *) params_void;

    schnittstelle->status = RUNNING;
    if (schnittstelle->interpreter->exec_script()) {
        schnittstelle->status = COMPLETED_OK;
    } else {
        schnittstelle->status = RUN_ERROR;
    }
    schnittstelle->is_running = false;

    return nullptr;
}

void Schnittstelle::start_script(const std::string &script) {
    kill_current_task();

    status = LOADING;
    if (!interpreter->load_script(script)) {
        status = LOAD_ERROR;
        return;
    }
    clear_error();

    // create and start thread
    is_running = true;
    pthread_create(&exec_thread, nullptr, Schnittstelle::exec_script, this);
}

void Schnittstelle::set_language(LANG lang) {
    current_language = lang;
    init_interpreter();
}

void Schnittstelle::init_interpreter() {
    kill_current_task();
    delete interpreter;

    switch (current_language) {
        case BASIC:
            interpreter = new BasicPlugin(
                    draw_function,
                    clear_function,
                    print_function
            );
            break;
        case LUA:
            interpreter = new LuaPlugin(
                    draw_function,
                    clear_function,
                    print_function
            );
            break;
        default:
            throw std::runtime_error("No interpreter!");
    }
}


Schnittstelle::Status Schnittstelle::get_status() {
    return status;
}

void Schnittstelle::kill_current_task() {
    if (is_running) {
        printf("Killing...");
        pthread_cancel(exec_thread);
        pthread_join(exec_thread, nullptr);
        is_running = false;
        printf("Killed!\n");
    }
}

Schnittstelle::Schnittstelle(
        LANG lang,
        print_funct_t print_function_value,
        draw_funct_t draw_function_value,
        clear_funct_t clear_function_value
) :
        current_language(lang),
        print_function(print_function_value),
        draw_function(draw_function_value),
        clear_function(clear_function_value) {
    init_interpreter();
    help_root_entry = initHelpSystem("../help_data/");
}

void Schnittstelle::save(const std::string &name, const std::string &text) {
    std::ofstream outfile;
    std::string extension;
    switch (current_language) {
        case BASIC:
            extension = ".bas";
            break;
        case LUA:
            extension = ".lua";
            break;
    }
    outfile.open("../saves/" + name + extension);
    outfile << text;
    outfile.close();
}

std::string Schnittstelle::load(const std::string &name) {
    std::ifstream infile;
    std::string extension;
    switch (current_language) {
        case BASIC:
            extension = ".bas";
            break;
        case LUA:
            extension = ".lua";
            break;
    }
    infile.open("../saves/" + name + extension);
    return std::string(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
}

Entry *Schnittstelle::get_common_help_root() {
    for (auto sub_entry = help_root_entry.sub_entries.begin(); sub_entry != help_root_entry.sub_entries.end(); ++sub_entry) {
        if (sub_entry->name == "Common") {
            return sub_entry.base();
        }
    }
    return nullptr;
}

Entry *Schnittstelle::get_language_help_root() {
    for (auto sub_entry = help_root_entry.sub_entries.begin(); sub_entry != help_root_entry.sub_entries.end(); ++sub_entry) {
        switch (current_language) {
            case BASIC:
                if (sub_entry->name == "BASIC") {
                    return sub_entry.base();
                }
                break;
            case LUA:
                if (sub_entry->name == "Lua") {
                    return sub_entry.base();
                }
                break;
        }
    }
    return nullptr;
}

std::vector<Entry *> Schnittstelle::search_entries(const std::string &searchword) {
    std::vector<Entry *> entries = searchEntries(get_common_help_root(), searchword);

    const std::vector<Entry *> &found = searchEntries(get_language_help_root(), searchword);
    entries.insert(entries.end(), found.begin(), found.end());

    return entries;
}
