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

bool Schnittstelle::start_script(const std::string &script) {
    kill_current_task();

    status = LOADING;
    if (!interpreter->load_script(script)) {
        status = LOAD_ERROR;
        return false;
    }
    gui->editor->clear_error_markers();

    // create and start thread
    status = RUNNING;
    pthread_create(&exec_thread, nullptr, Schnittstelle::exec_script, nullptr);
    return true;
}

bool Schnittstelle::handle_command(std::string command) {
    if (status == RUNNING) {
        if (command == "stop") {
            Schnittstelle::kill_current_task();
            gui->console->print("OK");
            return true;
        }
        if (Plugin::waiting_for_input) {
            Plugin::input = command;
            Plugin::input_ready = true;
            return true;
        }
    }
    if (command == "start" || command == "run") {
        if (Schnittstelle::start_script(gui->editor->get_text())) {
            gui->console->print("OK");
        }
        return true;
    }
    if (command == "stop") {
        Schnittstelle::kill_current_task();
        gui->console->print("OK");
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
            gui->console->print("OK");
            return true;
        } else {
            gui->editor->set_text(Schnittstelle::load(name));
            gui->console->print("OK");
            return true;
        }
    }
    return false;
}


void Schnittstelle::on_key_press(const SDL_Keysym keysym) {
    const std::string &key_name = get_key_name(keysym);
    interpreter->on_key_press(key_name);
}

void Schnittstelle::on_key_release(const SDL_Keysym keysym) {
    const std::string &key_name = get_key_name(keysym);
    interpreter->on_key_release(key_name);
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
        if (sub_entry->name == "Common") {
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
    std::sort(entries.begin(), entries.end(), [&](const Entry *a, const Entry *b) {
        auto a_iter = a->name.begin();
        auto b_iter = b->name.begin();
        auto s_iter = searchword.begin();

        // count how many letters match the start of a word until one word ends
        int a_count, b_count;
        for (a_count = 0; tolower(*a_iter) == *s_iter && a_iter != a->name.end() && s_iter != searchword.end(); a_iter++, s_iter++, a_count++);
        for (b_count = 0; tolower(*b_iter) == *s_iter && b_iter != a->name.end() && s_iter != searchword.end(); b_iter++, s_iter++, b_count++);


        return a_count > b_count;
    });

    return entries;
}


void Schnittstelle::gui_draw(int x, int y, int red, int green, int blue, int alpha, int size) {
    gui->graphic->add_pixel(x, y, red, green, blue, alpha, size);
}

void Schnittstelle::gui_clear() {
    gui->graphic->clear_pixels();
}

void Schnittstelle::gui_print(const std::string &message) {
    gui->console->print(message);
}

void Schnittstelle::on_error(int line, const std::string &message) {
    gui->console->print("[error] " + message);
    gui->editor->set_error_marker(line, message);
}


std::string Schnittstelle::get_key_name(const SDL_Keysym &keysym) {
    switch (keysym.scancode) {
        default:
            return "unknown";
        case SDL_SCANCODE_A:
            return "a";
        case SDL_SCANCODE_B:
            return "b";
        case SDL_SCANCODE_C:
            return "c";
        case SDL_SCANCODE_D:
            return "d";
        case SDL_SCANCODE_E:
            return "e";
        case SDL_SCANCODE_F:
            return "f";
        case SDL_SCANCODE_G:
            return "g";
        case SDL_SCANCODE_H:
            return "h";
        case SDL_SCANCODE_I:
            return "i";
        case SDL_SCANCODE_J:
            return "j";
        case SDL_SCANCODE_K:
            return "k";
        case SDL_SCANCODE_L:
            return "l";
        case SDL_SCANCODE_M:
            return "m";
        case SDL_SCANCODE_N:
            return "n";
        case SDL_SCANCODE_O:
            return "o";
        case SDL_SCANCODE_P:
            return "p";
        case SDL_SCANCODE_Q:
            return "q";
        case SDL_SCANCODE_R:
            return "r";
        case SDL_SCANCODE_S:
            return "s";
        case SDL_SCANCODE_T:
            return "t";
        case SDL_SCANCODE_U:
            return "u";
        case SDL_SCANCODE_V:
            return "v";
        case SDL_SCANCODE_W:
            return "w";
        case SDL_SCANCODE_X:
            return "x";
        case SDL_SCANCODE_Y:
            return "y";
        case SDL_SCANCODE_Z:
            return "z";
        case SDL_SCANCODE_1:
            return "1";
        case SDL_SCANCODE_2:
            return "2";
        case SDL_SCANCODE_3:
            return "3";
        case SDL_SCANCODE_4:
            return "4";
        case SDL_SCANCODE_5:
            return "5";
        case SDL_SCANCODE_6:
            return "6";
        case SDL_SCANCODE_7:
            return "7";
        case SDL_SCANCODE_8:
            return "8";
        case SDL_SCANCODE_9:
            return "9";
        case SDL_SCANCODE_0:
            return "0";
        case SDL_SCANCODE_RETURN:
            return "return";
        case SDL_SCANCODE_ESCAPE:
            return "escape";
        case SDL_SCANCODE_BACKSPACE:
            return "backspace";
        case SDL_SCANCODE_TAB:
            return "tab";
        case SDL_SCANCODE_SPACE:
            return "space";
        case SDL_SCANCODE_MINUS:
            return "minus";
        case SDL_SCANCODE_EQUALS:
            return "equals";
        case SDL_SCANCODE_LEFTBRACKET:
            return "leftbracket";
        case SDL_SCANCODE_RIGHTBRACKET:
            return "rightbracket";
        case SDL_SCANCODE_BACKSLASH:
            return "backslash";
        case SDL_SCANCODE_NONUSHASH:
            return "nonushash";
        case SDL_SCANCODE_SEMICOLON:
            return "semicolon";
        case SDL_SCANCODE_APOSTROPHE:
            return "apostrophe";
        case SDL_SCANCODE_GRAVE:
            return "grave";
        case SDL_SCANCODE_COMMA:
            return "comma";
        case SDL_SCANCODE_PERIOD:
            return "period";
        case SDL_SCANCODE_SLASH:
            return "slash";
        case SDL_SCANCODE_CAPSLOCK:
            return "capslock";
        case SDL_SCANCODE_F1:
            return "f1";
        case SDL_SCANCODE_F2:
            return "f2";
        case SDL_SCANCODE_F3:
            return "f3";
        case SDL_SCANCODE_F4:
            return "f4";
        case SDL_SCANCODE_F5:
            return "f5";
        case SDL_SCANCODE_F6:
            return "f6";
        case SDL_SCANCODE_F7:
            return "f7";
        case SDL_SCANCODE_F8:
            return "f8";
        case SDL_SCANCODE_F9:
            return "f9";
        case SDL_SCANCODE_F10:
            return "f10";
        case SDL_SCANCODE_F11:
            return "f11";
        case SDL_SCANCODE_F12:
            return "f12";
        case SDL_SCANCODE_PRINTSCREEN:
            return "printscreen";
        case SDL_SCANCODE_SCROLLLOCK:
            return "scrolllock";
        case SDL_SCANCODE_PAUSE:
            return "pause";
        case SDL_SCANCODE_INSERT:
            return "insert";
        case SDL_SCANCODE_HOME:
            return "home";
        case SDL_SCANCODE_PAGEUP:
            return "pageup";
        case SDL_SCANCODE_DELETE:
            return "delete";
        case SDL_SCANCODE_END:
            return "end";
        case SDL_SCANCODE_PAGEDOWN:
            return "pagedown";
        case SDL_SCANCODE_RIGHT:
            return "right";
        case SDL_SCANCODE_LEFT:
            return "left";
        case SDL_SCANCODE_DOWN:
            return "down";
        case SDL_SCANCODE_UP:
            return "up";
        case SDL_SCANCODE_NUMLOCKCLEAR:
            return "numlockclear";
        case SDL_SCANCODE_KP_DIVIDE:
            return "kp_divide";
        case SDL_SCANCODE_KP_MULTIPLY:
            return "kp_multiply";
        case SDL_SCANCODE_KP_MINUS:
            return "kp_minus";
        case SDL_SCANCODE_KP_PLUS:
            return "kp_plus";
        case SDL_SCANCODE_KP_ENTER:
            return "kp_enter";
        case SDL_SCANCODE_KP_1:
            return "kp_1";
        case SDL_SCANCODE_KP_2:
            return "kp_2";
        case SDL_SCANCODE_KP_3:
            return "kp_3";
        case SDL_SCANCODE_KP_4:
            return "kp_4";
        case SDL_SCANCODE_KP_5:
            return "kp_5";
        case SDL_SCANCODE_KP_6:
            return "kp_6";
        case SDL_SCANCODE_KP_7:
            return "kp_7";
        case SDL_SCANCODE_KP_8:
            return "kp_8";
        case SDL_SCANCODE_KP_9:
            return "kp_9";
        case SDL_SCANCODE_KP_0:
            return "kp_0";
        case SDL_SCANCODE_KP_PERIOD:
            return "kp_period";
        case SDL_SCANCODE_NONUSBACKSLASH:
            return "nonusbackslash";
        case SDL_SCANCODE_APPLICATION:
            return "application";
        case SDL_SCANCODE_POWER:
            return "power";
        case SDL_SCANCODE_KP_EQUALS:
            return "kp_equals";
        case SDL_SCANCODE_F13:
            return "f13";
        case SDL_SCANCODE_F14:
            return "f14";
        case SDL_SCANCODE_F15:
            return "f15";
        case SDL_SCANCODE_F16:
            return "f16";
        case SDL_SCANCODE_F17:
            return "f17";
        case SDL_SCANCODE_F18:
            return "f18";
        case SDL_SCANCODE_F19:
            return "f19";
        case SDL_SCANCODE_F20:
            return "f20";
        case SDL_SCANCODE_F21:
            return "f21";
        case SDL_SCANCODE_F22:
            return "f22";
        case SDL_SCANCODE_F23:
            return "f23";
        case SDL_SCANCODE_F24:
            return "f24";
        case SDL_SCANCODE_EXECUTE:
            return "execute";
        case SDL_SCANCODE_HELP:
            return "help";
        case SDL_SCANCODE_MENU:
            return "menu";
        case SDL_SCANCODE_SELECT:
            return "select";
        case SDL_SCANCODE_STOP:
            return "stop";
        case SDL_SCANCODE_AGAIN:
            return "again";
        case SDL_SCANCODE_UNDO:
            return "undo";
        case SDL_SCANCODE_CUT:
            return "cut";
        case SDL_SCANCODE_COPY:
            return "copy";
        case SDL_SCANCODE_PASTE:
            return "paste";
        case SDL_SCANCODE_FIND:
            return "find";
        case SDL_SCANCODE_MUTE:
            return "mute";
        case SDL_SCANCODE_VOLUMEUP:
            return "volumeup";
        case SDL_SCANCODE_VOLUMEDOWN:
            return "volumedown";
        case SDL_SCANCODE_KP_COMMA:
            return "kp_comma";
        case SDL_SCANCODE_KP_EQUALSAS400:
            return "kp_equalsas400";
        case SDL_SCANCODE_INTERNATIONAL1:
            return "international1";
        case SDL_SCANCODE_INTERNATIONAL2:
            return "international2";
        case SDL_SCANCODE_INTERNATIONAL3:
            return "international3";
        case SDL_SCANCODE_INTERNATIONAL4:
            return "international4";
        case SDL_SCANCODE_INTERNATIONAL5:
            return "international5";
        case SDL_SCANCODE_INTERNATIONAL6:
            return "international6";
        case SDL_SCANCODE_INTERNATIONAL7:
            return "international7";
        case SDL_SCANCODE_INTERNATIONAL8:
            return "international8";
        case SDL_SCANCODE_INTERNATIONAL9:
            return "international9";
        case SDL_SCANCODE_LANG1:
            return "lang1";
        case SDL_SCANCODE_LANG2:
            return "lang2";
        case SDL_SCANCODE_LANG3:
            return "lang3";
        case SDL_SCANCODE_LANG4:
            return "lang4";
        case SDL_SCANCODE_LANG5:
            return "lang5";
        case SDL_SCANCODE_LANG6:
            return "lang6";
        case SDL_SCANCODE_LANG7:
            return "lang7";
        case SDL_SCANCODE_LANG8:
            return "lang8";
        case SDL_SCANCODE_LANG9:
            return "lang9";
        case SDL_SCANCODE_ALTERASE:
            return "alterase";
        case SDL_SCANCODE_SYSREQ:
            return "sysreq";
        case SDL_SCANCODE_CANCEL:
            return "cancel";
        case SDL_SCANCODE_CLEAR:
            return "clear";
        case SDL_SCANCODE_PRIOR:
            return "prior";
        case SDL_SCANCODE_RETURN2:
            return "return2";
        case SDL_SCANCODE_SEPARATOR:
            return "separator";
        case SDL_SCANCODE_OUT:
            return "out";
        case SDL_SCANCODE_OPER:
            return "oper";
        case SDL_SCANCODE_CLEARAGAIN:
            return "clearagain";
        case SDL_SCANCODE_CRSEL:
            return "crsel";
        case SDL_SCANCODE_EXSEL:
            return "exsel";
        case SDL_SCANCODE_KP_00:
            return "kp_00";
        case SDL_SCANCODE_KP_000:
            return "kp_000";
        case SDL_SCANCODE_THOUSANDSSEPARATOR:
            return "thousandsseparator";
        case SDL_SCANCODE_DECIMALSEPARATOR:
            return "decimalseparator";
        case SDL_SCANCODE_CURRENCYUNIT:
            return "currencyunit";
        case SDL_SCANCODE_CURRENCYSUBUNIT:
            return "currencysubunit";
        case SDL_SCANCODE_KP_LEFTPAREN:
            return "kp_leftparen";
        case SDL_SCANCODE_KP_RIGHTPAREN:
            return "kp_rightparen";
        case SDL_SCANCODE_KP_LEFTBRACE:
            return "kp_leftbrace";
        case SDL_SCANCODE_KP_RIGHTBRACE:
            return "kp_rightbrace";
        case SDL_SCANCODE_KP_TAB:
            return "kp_tab";
        case SDL_SCANCODE_KP_BACKSPACE:
            return "kp_backspace";
        case SDL_SCANCODE_KP_A:
            return "kp_a";
        case SDL_SCANCODE_KP_B:
            return "kp_b";
        case SDL_SCANCODE_KP_C:
            return "kp_c";
        case SDL_SCANCODE_KP_D:
            return "kp_d";
        case SDL_SCANCODE_KP_E:
            return "kp_e";
        case SDL_SCANCODE_KP_F:
            return "kp_f";
        case SDL_SCANCODE_KP_XOR:
            return "kp_xor";
        case SDL_SCANCODE_KP_POWER:
            return "kp_power";
        case SDL_SCANCODE_KP_PERCENT:
            return "kp_percent";
        case SDL_SCANCODE_KP_LESS:
            return "kp_less";
        case SDL_SCANCODE_KP_GREATER:
            return "kp_greater";
        case SDL_SCANCODE_KP_AMPERSAND:
            return "kp_ampersand";
        case SDL_SCANCODE_KP_DBLAMPERSAND:
            return "kp_dblampersand";
        case SDL_SCANCODE_KP_VERTICALBAR:
            return "kp_verticalbar";
        case SDL_SCANCODE_KP_DBLVERTICALBAR:
            return "kp_dblverticalbar";
        case SDL_SCANCODE_KP_COLON:
            return "kp_colon";
        case SDL_SCANCODE_KP_HASH:
            return "kp_hash";
        case SDL_SCANCODE_KP_SPACE:
            return "kp_space";
        case SDL_SCANCODE_KP_AT:
            return "kp_at";
        case SDL_SCANCODE_KP_EXCLAM:
            return "kp_exclam";
        case SDL_SCANCODE_KP_MEMSTORE:
            return "kp_memstore";
        case SDL_SCANCODE_KP_MEMRECALL:
            return "kp_memrecall";
        case SDL_SCANCODE_KP_MEMCLEAR:
            return "kp_memclear";
        case SDL_SCANCODE_KP_MEMADD:
            return "kp_memadd";
        case SDL_SCANCODE_KP_MEMSUBTRACT:
            return "kp_memsubtract";
        case SDL_SCANCODE_KP_MEMMULTIPLY:
            return "kp_memmultiply";
        case SDL_SCANCODE_KP_MEMDIVIDE:
            return "kp_memdivide";
        case SDL_SCANCODE_KP_PLUSMINUS:
            return "kp_plusminus";
        case SDL_SCANCODE_KP_CLEAR:
            return "kp_clear";
        case SDL_SCANCODE_KP_CLEARENTRY:
            return "kp_clearentry";
        case SDL_SCANCODE_KP_BINARY:
            return "kp_binary";
        case SDL_SCANCODE_KP_OCTAL:
            return "kp_octal";
        case SDL_SCANCODE_KP_DECIMAL:
            return "kp_decimal";
        case SDL_SCANCODE_KP_HEXADECIMAL:
            return "kp_hexadecimal";
        case SDL_SCANCODE_LCTRL:
            return "lctrl";
        case SDL_SCANCODE_LSHIFT:
            return "lshift";
        case SDL_SCANCODE_LALT:
            return "lalt";
        case SDL_SCANCODE_LGUI:
            return "lgui";
        case SDL_SCANCODE_RCTRL:
            return "rctrl";
        case SDL_SCANCODE_RSHIFT:
            return "rshift";
        case SDL_SCANCODE_RALT:
            return "ralt";
        case SDL_SCANCODE_RGUI:
            return "rgui";
        case SDL_SCANCODE_MODE:
            return "mode";
        case SDL_SCANCODE_AUDIONEXT:
            return "audionext";
        case SDL_SCANCODE_AUDIOPREV:
            return "audioprev";
        case SDL_SCANCODE_AUDIOSTOP:
            return "audiostop";
        case SDL_SCANCODE_AUDIOPLAY:
            return "audioplay";
        case SDL_SCANCODE_AUDIOMUTE:
            return "audiomute";
        case SDL_SCANCODE_MEDIASELECT:
            return "mediaselect";
        case SDL_SCANCODE_WWW:
            return "www";
        case SDL_SCANCODE_MAIL:
            return "mail";
        case SDL_SCANCODE_CALCULATOR:
            return "calculator";
        case SDL_SCANCODE_COMPUTER:
            return "computer";
        case SDL_SCANCODE_AC_SEARCH:
            return "ac_search";
        case SDL_SCANCODE_AC_HOME:
            return "ac_home";
        case SDL_SCANCODE_AC_BACK:
            return "ac_back";
        case SDL_SCANCODE_AC_FORWARD:
            return "ac_forward";
        case SDL_SCANCODE_AC_STOP:
            return "ac_stop";
        case SDL_SCANCODE_AC_REFRESH:
            return "ac_refresh";
        case SDL_SCANCODE_AC_BOOKMARKS:
            return "ac_bookmarks";
        case SDL_SCANCODE_BRIGHTNESSDOWN:
            return "brightnessdown";
        case SDL_SCANCODE_BRIGHTNESSUP:
            return "brightnessup";
        case SDL_SCANCODE_DISPLAYSWITCH:
            return "displayswitch";
        case SDL_SCANCODE_KBDILLUMTOGGLE:
            return "kbdillumtoggle";
        case SDL_SCANCODE_KBDILLUMDOWN:
            return "kbdillumdown";
        case SDL_SCANCODE_KBDILLUMUP:
            return "kbdillumup";
        case SDL_SCANCODE_EJECT:
            return "eject";
        case SDL_SCANCODE_SLEEP:
            return "sleep";
        case SDL_SCANCODE_APP1:
            return "app1";
        case SDL_SCANCODE_APP2:
            return "app2";
        case SDL_SCANCODE_AUDIOREWIND:
            return "audiorewind";
        case SDL_SCANCODE_AUDIOFASTFORWARD:
            return "audiofastforward";
    }
}
