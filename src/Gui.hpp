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

    int initialize();
    void destroy();
    int tick();

private:
    /* SDL and ImGui State */
    bool show_demo_window;
    SDL_Window *window;
    SDL_GLContext gl_context;
    ImVec4 clear_color;

    /* gui elements */
    std::vector<UiElement*> uiElements;

    int init_sdl();
    void init_imgui();
    void build_windows();
    void render_windows();
};

#endif //IMGUI_SETUP_GUI_HPP
