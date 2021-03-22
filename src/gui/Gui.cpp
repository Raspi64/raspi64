//
// Created by Alex on 15.11.20.
//

// Dear ImGui: standalone example application for SDL2 + OpenGL
// (SDL is a cross-platform general purpose library for handling windows, inputs, OpenGL/Vulkan/Metal graphics context creation, etc.)
// If you are new to Dear ImGui, read documentation from the docs/ folder + read the top of imgui.cpp.
// Read online: https://github.com/ocornut/imgui/tree/master/docs

// **DO NOT USE THIS CODE IF YOUR CODE/ENGINE IS USING MODERN OPENGL (SHADERS, VBO, VAO, etc.)**
// **Prefer using the code in the example_sdl_opengl3/ folder**
// See imgui_impl_sdl.cpp for details.
#include "imgui.h"
#include "imgui_impl_sdl.h"
#include "imgui_impl_opengl2.h"
#include <stdio.h>
#include <vector>
#include <SDL.h>
#include <SDL_opengl.h>
#include <iostream>
#include <debug.h>

#include "Gui.hpp"
#include "ui/UiElement.hpp"
#include "ui/MainMenuBar.hpp"
#include "ui/window/EditorWindow.hpp"
#include "ui/window/HelpWindow.hpp"
#include "ui/window/GraphicWindow.hpp"
#include "ui/window/ConsoleWindow.hpp"
#include "gui/ui/window/dialog/ShutdownDialogWindow.hpp"
#include "gui/ui/window/dialog/ChangeLangModeDialogWindow.hpp"
#include "config.hpp"


Gui::Gui() :
        show_demo_window(true),
        window(nullptr),
        uiElements(),
        on_keydown_fn(nullptr),
        editorWindow(nullptr),
        helpWindow(nullptr) {
    set_language_mode(LUA);
}

int Gui::initialize() {
    int sdl = init_sdl();
    if (sdl != 0) return sdl;

    init_imgui();

    clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Our state
    // bool show_app_console = true;
    show_demo_window = true;

    build_windows();

    return 0;
}

int Gui::init_sdl() {
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0) {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
//    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);
    SDL_WindowFlags window_flags = (SDL_WindowFlags) (
            SDL_WINDOW_OPENGL | SDL_WINDOW_FULLSCREEN_DESKTOP);
    window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED, 640, 360, window_flags);
    gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(0); // Enable vsync
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);

    return 0;
}

void Gui::init_imgui() {
    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    //ImGui::StyleColorsDark();
    ImGui::StyleColorsClassic();

    // Setup Platform/Renderer backends
    ImGui_ImplSDL2_InitForOpenGL(window, gl_context);
    ImGui_ImplOpenGL2_Init();

    // Load Fonts
    // - If no fonts are loaded, dear imgui will use the default font. You can also load multiple fonts and use ImGui::PushFont()/PopFont() to select them.
    // - AddFontFromFileTTF() will return the ImFont* so you can store it if you need to select the font among multiple.
    // - If the file cannot be loaded, the function will return NULL. Please handle those errors in your application (e.g. use an assertion, or display an error and quit).
    // - The fonts will be rasterized at a given size (w/ oversampling) and stored into a texture when calling ImFontAtlas::Build()/GetTexDataAsXXXX(), which ImGui_ImplXXXX_NewFrame below will call.
    // - Read 'docs/FONTS.md' for more instructions and details.
    // - Remember that in C/C++ if you want to include a backslash \ in a string literal you need to write a double backslash \\ !
    //io.Fonts->AddFontDefault();
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Roboto-Medium.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/Cousine-Regular.ttf", 15.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/DroidSans.ttf", 16.0f);
    //io.Fonts->AddFontFromFileTTF("../../misc/fonts/ProggyTiny.ttf", 10.0f);
    //ImFont* font = io.Fonts->AddFontFromFileTTF("c:\\Windows\\Fonts\\ArialUni.ttf", 18.0f, NULL, io.Fonts->GetGlyphRangesJapanese());
    //IM_ASSERT(font != NULL);
}

void Gui::destroy() {
    // TODO
    /*
    for (size_t i = 0; i < uiElements.size(); i++) {
        UiElement* elem = uiElements[i];
        //delete elem;
    }
    */

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();
}

int Gui::tick() {
    struct timespec now, later;
    __syscall_slong_t diff_ns;


    // Poll and handle events (inputs, window resize, etc.)
    // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
    // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
    // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
    // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
    START_TIMING;
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        ImGui_ImplSDL2_ProcessEvent(&event);
        if (event.type == SDL_QUIT)
            return -1;
        if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE &&
            event.window.windowID == SDL_GetWindowID(window))
            return -1;

        if (Window::current_window == WIN_TITLE_GRAPHIC) {
            if (event.type == SDL_KEYDOWN && event.key.repeat == 0) {
                if (on_keydown_fn != nullptr)
                    on_keydown_fn(event.key.keysym);
            }
            if (event.type == SDL_KEYUP && event.key.repeat == 0) {
                if (on_keyup_fn != nullptr)
                    on_keyup_fn(event.key.keysym);
            }
        }
    }

    END_TIMING(Events);

    START_TIMING;
    // Start the Dear ImGui frame
    ImGui_ImplOpenGL2_NewFrame();
    ImGui_ImplSDL2_NewFrame(window);
    ImGui::NewFrame();
    END_TIMING(NewFrame);

    // 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
    //ImGui::ShowDemoWindow(&show_demo_window);

    TIME(check_keys())

    // Create the needed windows
    TIME(render_windows());

    // Rendering
    TIME(ImGui::Render());

    START_TIMING;

    ImGuiIO &io = ImGui::GetIO();
    glViewport(0, 0, (int) io.DisplaySize.x, (int) io.DisplaySize.y);
    glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
    glClear(GL_COLOR_BUFFER_BIT);
    //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
    END_TIMING(IO);

    TIME(ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData()))

    TIME(SDL_GL_SwapWindow(window))

    return 0;
}

void Gui::build_windows() {
    uiElements.push_back(new MainMenuBar(&current_language));

    this->editorWindow = new EditorWindow(current_language);
    editor = new EditorWindowHandler(this->editorWindow);
    uiElements.push_back(this->editorWindow);

    this->helpWindow = new HelpWindow(current_language);
    help = new HelpWindowHandler(this->helpWindow);
    uiElements.push_back(this->helpWindow);

    GraphicWindow *graphicWindow = new GraphicWindow();
    graphic = new GraphicWindowHandler(graphicWindow);
    uiElements.push_back(graphicWindow);

    ConsoleWindow *consoleWindow = new ConsoleWindow();
    console = new ConsoleWindowHandler(consoleWindow);
    uiElements.push_back(consoleWindow);

    uiElements.push_back(new ShutdownDialogWindow());


    /*
    std::function<void(LANG)> cb = [=](LANG newLang) {
        printf("callback called");
        this->on_change_langmode_request_fn(newLang);
    };
    ChangeLangModeDialogWindow* changeLangModeDialogWindow = new ChangeLangModeDialogWindow(cb);
    */

    ChangeLangModeDialogWindow *changeLangModeDialogWindow = new ChangeLangModeDialogWindow([=](LANG newLang) {
        printf("callback called");
        if (on_change_langmode_request_fn != nullptr)
            this->on_change_langmode_request_fn(newLang);
    }, &current_language);

    uiElements.push_back(changeLangModeDialogWindow);
}

void Gui::render_windows() {
    struct timespec now, later;
    __syscall_slong_t diff_ns;

    for (size_t i = 0; i < uiElements.size(); i++) {
        START_TIMING;
        uiElements[i]->render();
        END_TIMING(Component);
    }
}

void Gui::check_keys() {
    if (ImGui::IsKeyPressed(FOCUS_KEY_HELP))
        ImGui::SetWindowFocus(WIN_TITLE_HELP);
    else if (ImGui::IsKeyPressed(FOCUS_KEY_EDITOR))
        ImGui::SetWindowFocus(WIN_TITLE_EDITOR);
    else if (ImGui::IsKeyPressed(FOCUS_KEY_GRAPHIC))
        ImGui::SetWindowFocus(WIN_TITLE_GRAPHIC);
    else if (ImGui::IsKeyPressed(FOCUS_KEY_CONSOLE))
        ImGui::SetWindowFocus(WIN_TITLE_CONSOLE);
    else if (ImGui::IsKeyPressed(TRIGGER_KEY_SHUTDOWN))
        ImGui::OpenPopup(WIN_TITLE_SHUTDOWN);
    else if (ImGui::IsKeyPressed(TRIGGER_KEY_CHANGELANGMODE))
        ImGui::OpenPopup(WIN_TITLE_CHANGELANGMODE);
}

void Gui::on_keydown(keydown_funct_t function) {
    this->on_keydown_fn = function;
}

void Gui::on_keyup(keydown_funct_t function) {
    this->on_keyup_fn = function;
}

void Gui::on_change_langmode_request(change_langmode_request_func_t function) {
    this->on_change_langmode_request_fn = function;
}

void Gui::set_language_mode(LANG lang) {
    current_language = lang;

    // update windows
    if (this->editorWindow != nullptr)
        this->editorWindow->set_language_mode(lang);
    if (this->helpWindow != nullptr)
        this->helpWindow->set_language_mode(lang);
}
