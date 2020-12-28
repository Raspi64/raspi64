//
// Created by simon on 12/1/20.
//

#include <iostream>
#include <fstream>
#include <algorithm>
#include <filesystem>
#include "BasicPlugin.hpp"
#include "LuaPlugin.hpp"
#include "Schnittstelle.hpp"
#include "HelpSystem.hpp"


Gui *Schnittstelle::gui;
Plugin *Schnittstelle::interpreter;
pthread_t Schnittstelle::exec_thread;
Entry Schnittstelle::help_root_entry;
Schnittstelle::Status Schnittstelle::status;


void *Schnittstelle::exec_script(void *) {
    if (Schnittstelle::interpreter->exec_script()) {
        Schnittstelle::status = COMPLETED_OK;
    } else {
        Schnittstelle::status = RUN_ERROR;
    }

    return nullptr;
}

void Schnittstelle::start_script(const std::string &script) {
    kill_current_task();

    status = LOADING;
    if (!interpreter->load_script(script)) {
        status = LOAD_ERROR;
        return;
    }
    gui->editor->clear_error_markers();

    // create and start thread
    status = RUNNING;
    pthread_create(&exec_thread, nullptr, Schnittstelle::exec_script, nullptr);
}

bool Schnittstelle::handle_command(std::string command) {
    if (command == "start" || command == "run") {
        Schnittstelle::start_script(gui->editor->get_text());
        return true;
    }
    if (command == "stop") {
        Schnittstelle::kill_current_task();
        return true;
    }
    if (command.find("list") == 0) {
        gui->console->print("Files you can load:");
        for (auto &child: std::filesystem::directory_iterator("saves/")) {
            if (!child.is_directory()) {
                std::string child_path = child.path();
                std::string name(child_path.substr(child_path.rfind('/') + 1));
                gui->console->print(" - " + name);
            }
        }
        return true;
    }
    if (command.find("save") == 0 || command.find("load") == 0) {
        unsigned long pos = command.find(' ');
        if (pos == std::string::npos) {
            gui->console->print("[error] Kein Dateinamen gefunden");
            return true;
        }
        std::string name = command.substr(pos + 1);
        if (name.find(' ') != std::string::npos) {
            gui->console->print("[error] Dateiname darf keine Leerzeichen enthalten");
            return true;
        }
        if (name.find('/') != std::string::npos) {
            gui->console->print("[error] Dateiname darf keine Slashes enthalten");
            return true;
        }
        if (name.find('.') != std::string::npos) {
            gui->console->print("[error] Dateiname darf keine Punkte enthalten");
            gui->console->print("(Die Dateiendung fuegt das Programm selbst hinzu)");
//            gui->console->print("Wenn du ein %s Programm laden moechtest, wechsele erst den Modus.", sc.get_language() == LUA ? "Basic" : "Lua");
            return true;
        }
        if (command[0] == 's') {
            Schnittstelle::save(name, gui->editor->get_text());
            return true;
        } else {
            gui->editor->set_text(Schnittstelle::load(name));
            return true;
        }
    }
    return false;
}

void Schnittstelle::set_language(LANG lang) {
    kill_current_task();
    interpreter = get_interpreter(lang);
    gui->set_language_mode(lang);
}

Plugin *Schnittstelle::get_interpreter(LANG language) {
    switch (language) {
        case BASIC:
            return new BasicPlugin();
        case LUA:
            return new LuaPlugin();
        default:
            throw std::runtime_error("No interpreter!");
    }
}

void Schnittstelle::kill_current_task() {
    if (status == RUNNING) {
        printf("Killing...");
        pthread_cancel(exec_thread);
        pthread_join(exec_thread, nullptr);
        status = KILLED;
        printf("Killed!\n");
    }
}

void Schnittstelle::init(Gui *ui, LANG lang) {
    Schnittstelle::gui = ui;

    interpreter = get_interpreter(lang);
    help_root_entry = initHelpSystem("help_data/");

    sort_subtrees(&help_root_entry.sub_entries);
}

void Schnittstelle::sort_subtrees(std::vector<Entry> *entries) {
    std::sort(entries->begin(), entries->end(), [&](const Entry &a, const Entry &b) { return a.name.compare(b.name) < 0; });
    for (auto &entry : *entries) {
        sort_subtrees(&entry.sub_entries);
    }
}

void Schnittstelle::save(const std::string &name, const std::string &text) {
    std::ofstream outfile;
    outfile.open("saves/" + name + interpreter->get_extension());
    outfile << text;
    outfile.close();
}

std::string Schnittstelle::load(const std::string &name) {
    std::ifstream infile;
    infile.open("saves/" + name + interpreter->get_extension());
    return std::string(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
}

Entry *Schnittstelle::get_common_help_root() {
    for (auto sub_entry = help_root_entry.sub_entries.begin(); sub_entry != help_root_entry.sub_entries.end(); ++sub_entry) {
        if (sub_entry->name == interpreter->get_help_folder_name()) {
            return sub_entry.base();
        }
    }
    return nullptr;
}

Entry *Schnittstelle::get_language_help_root() {
    for (auto sub_entry = help_root_entry.sub_entries.begin(); sub_entry != help_root_entry.sub_entries.end(); ++sub_entry) {
	    if (sub_entry->name == interpreter->get_help_folder_name()) {
		    return sub_entry.base();
	    }
    }
    return nullptr;
}

std::vector<Entry *> Schnittstelle::search_entries(const std::string &searchword) {
    std::vector<Entry *> entries = searchEntries(get_common_help_root(), searchword);

    const std::vector<Entry *> &found = searchEntries(get_language_help_root(), searchword);
    entries.insert(entries.end(), found.begin(), found.end());

    // Eintraege alphabetisch sortieren
    std::sort(entries.begin(), entries.end(), [&](const Entry *a, const Entry *b) {
        return a->name.compare(b->name) < 0;
    });

    // Eintraege, die mit dem Suchwort beginnen nach vorne stellen (alle Anderen bleiben unveraendert)
//    std::cout << searchword << std::endl;
    std::sort(entries.begin(), entries.end(), [&](const Entry *a, const Entry *b) {
        auto a_iter = a->name.begin();
        auto b_iter = b->name.begin();
        auto s_iter = searchword.begin();

        // count how many letters match the start of a word until one word ends
        int a_count, b_count;
        for (a_count = 0; tolower(*a_iter) == *s_iter && a_iter != a->name.end() && s_iter != searchword.end(); a_iter++, s_iter++, a_count++);
        for (b_count = 0; tolower(*b_iter) == *s_iter && b_iter != a->name.end() && s_iter != searchword.end(); b_iter++, s_iter++, b_count++);
//        std::cout << "a:" << a->name << ": " << a_count << "\tb:" << b->name << ": " << b_count << std::endl;


        return a_count > b_count;
    });

    return entries;
}


void Schnittstelle::gui_draw(int x, int y, int red, int green, int blue, int alpha, int size) {
    gui->graphic->add_pixel(x, y, red, green, blue, alpha, size);
//    std::cout << "GUI Draw!" << std::endl;
}

void Schnittstelle::gui_clear() {
    gui->graphic->clear_pixels();
//    std::cout << "GUI Clear!" << std::endl;
}

void Schnittstelle::gui_print(const std::string &message) {
    gui->console->print(message);
    std::cout << "Print: " << message << std::endl;
}

void Schnittstelle::on_error(int line, const std::string &message) {
    gui->console->print("[error] " + message);
    gui->editor->set_error_marker(line, message);
}
