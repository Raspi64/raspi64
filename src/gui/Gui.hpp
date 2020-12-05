//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_GUI_HPP
#define IMGUI_SETUP_GUI_HPP

#include <vector>
#include <SDL.h>
#include "imgui.h"

#include "ui/UiElement.hpp"
#include "ui/window/EditorWindowHandler.hpp"
#include "ui/window/HelpWindowHandler.hpp"
#include "ui/window/GraphicWindowHandler.hpp"
#include "ui/window/ConsoleWindowHandler.hpp"


class Gui {
public:
    Gui();
    EditorWindowHandler* editor;
    HelpWindowHandler* help;
    GraphicWindowHandler* graphic;
    ConsoleWindowHandler* console;

    /**
     * callback on key down.
     * Only gets called when focus is in graphical viewport
     */
    void on_keydown(keydown_funct_t function); // TODO: refactor: move to graphic window
    void on_change_langmode_request(change_langmode_request_func_t function);

    int initialize();
    void destroy();
    int tick();

    void set_language_mode(LANG lang);

private:
    /* SDL and ImGui State */
    bool show_demo_window;
    SDL_Window *window;
    SDL_GLContext gl_context;
    ImVec4 clear_color;

    /* gui elements */
    std::vector<UiElement*> uiElements;

    /* Callbacks */
    keydown_funct_t on_keydown_fn;
    change_langmode_request_func_t on_change_langmode_request_fn;



    int init_sdl();
    void init_imgui();
    void build_windows();
    void render_windows();
    void check_keys();
};

#endif //IMGUI_SETUP_GUI_HPP
