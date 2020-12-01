//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_CONFIG_HPP
#define IMGUI_SETUP_CONFIG_HPP

#define FLAGS_BASE (0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)
#define FLAGS_EDITOR FLAGS_BASE | ImGuiWindowFlags_HorizontalScrollbar
#define FLAGS_HELP (FLAGS_BASE)
#define FLAGS_CANVAS (FLAGS_BASE)
#define FLAGS_CONSOLE (FLAGS_BASE)

#define KEY_ESC 41
#define KEY_ENTER 40
#define KEY_F1 58
#define KEY_F2 59
#define KEY_F3 60
#define KEY_F4 61
#define KEY_F12 69

#define FOCUS_KEY_HELP              KEY_F1
#define FOCUS_KEY_HELP_DISPLAY      "F1"
#define FOCUS_KEY_EDITOR            KEY_F2
#define FOCUS_KEY_EDITOR_DISPLAY    "F2"
#define FOCUS_KEY_GRAPHIC           KEY_F3
#define FOCUS_KEY_GRAPHIC_DISPLAY   "F3"
#define FOCUS_KEY_CONSOLE           KEY_F4
#define FOCUS_KEY_CONSOLE_DISPLAY   "F4"

#define TRIGGER_KEY_SHUTDOWN            KEY_F12
#define TRIGGER_KEY_SHUTDOWN_DISPLAY    "F12"

#define WIN_TITLE_HELP      "Hilfe [ " FOCUS_KEY_HELP_DISPLAY " ]"
#define WIN_TITLE_EDITOR    "Editor [ " FOCUS_KEY_EDITOR_DISPLAY " ]"
#define WIN_TITLE_GRAPHIC   "Graphical Output [ " FOCUS_KEY_GRAPHIC_DISPLAY " ]"
#define WIN_TITLE_CONSOLE   "Konsole [ " FOCUS_KEY_CONSOLE_DISPLAY " ]"

#define WIN_TITLE_SHUTDOWN  "System herunterfahren"


#endif //IMGUI_SETUP_CONFIG_HPP
