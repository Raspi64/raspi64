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
#define FLAGS_BASE (0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)
#define FLAGS_EDITOR (FLAGS_BASE)
#define FLAGS_HELP (FLAGS_BASE)
#define FLAGS_CANVAS (FLAGS_BASE)
#define FLAGS_CONSOLE (FLAGS_BASE)




struct TGraphicPixel {
    int size;
    int x;
    int y;
    int red;
    int green;
    int blue;
    int alpha;
};

std::vector<TGraphicPixel> graphicPixels;

 void createMainMenuBar() 
 {
        if (ImGui::BeginMainMenuBar())
        {

            ImGui::Text("HELP - Hilfe anzeigen ");

            ImGui::Text("CRTL + C - Programm beenden ");

            ImGui::Text("RUN - Programm starten ");


            ImGui::EndMainMenuBar();
        }
 }


 void createEditor() 
 {
        ImGui::SetNextWindowPos(ImVec2(60, 50), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(700, 950), ImGuiCond_None);
    
        
        ImGui::Begin("Editor (F1 - Taste)", NULL, FLAGS_EDITOR);                          // Create a window called "Hello, world!" and append into it.
            
            
            
            static char text[1024 * 16] = 
            "10 PRINT HELLO WORLD\n"
            "20 START\n"
            "30 RESET";
            static ImGuiInputTextFlags flags = ImGuiInputTextFlags_AllowTabInput;
            ImGui::InputTextMultiline("##source", text, IM_ARRAYSIZE(text), ImVec2(-FLT_MIN, 915), flags);
        
        
        ImGui::End();
 } 

 void createHelpwindow() 
 {     
        ImGui::SetNextWindowPos(ImVec2(820, 50), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);
        
        //Help window
        ImGui::Begin("Help Window (F3 - Taste)", NULL, FLAGS_HELP);
        static char buf[32] = "Ägypten ÜÖÄ"; 
        ImGui::InputText("Search", buf, IM_ARRAYSIZE(buf));
        ImGui::TextWrapped(
        "\nThis text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        "This text is just a Test\n"
        );
        ImGui::End();
 }
 
 void drawGraphicPixels(ImVec2 origin, ImDrawList* draw_list) 
 {
      for(size_t i = 0; i < graphicPixels.size(); i++) 
        {
            
            TGraphicPixel t = graphicPixels[i];
            int offset = (int) t.size / 2;
            draw_list->AddRectFilled(ImVec2(origin.x + t.x - offset, origin.y + t.y - offset), ImVec2(origin.x + t.x + std::max(1,offset), origin.y + t.y + std::max(1,offset)), IM_COL32(t.red, t.green, t.blue, t.alpha));
        }
 }
 
 void createGraphicalOutput() 
 {
        ImGui::SetNextWindowPos(ImVec2(1350, 50), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);
    
        ImGui::Begin("Graphical Output (F4 - Taste)", NULL, FLAGS_CANVAS);   
        
       
        static ImVec2 scrolling(0.0f, 0.0f);
        static bool opt_enable_grid = true;
      
        
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
        
        // Draw border and background color
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
        
        // This will catch our interactions
        ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);
        
        
        
        // Draw grid + all lines in the canvas
        draw_list->PushClipRect(canvas_p0, canvas_p1, true);
        if (opt_enable_grid)
        {
            const float GRID_STEP = 16.0f;
            for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y), IM_COL32(200, 200, 200, 40));
            for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
                draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y), IM_COL32(200, 200, 200, 40));
        }
        
        drawGraphicPixels(origin, draw_list);
       
        draw_list->PopClipRect();
        
        ImGui::End();
 } 
 

 
 void drawPixel(int x, int y, int red, int green, int blue, int alpha = 255, int size = 1) 
 {
     TGraphicPixel test;
     test.size = size;
     test.x = x;
     test.y = y;
     test.red = red;
     test.green = green; 
     test.blue = blue;
     test.alpha = alpha;
     
     graphicPixels.push_back(test); 
     
     for(size_t i = 0; i <graphicPixels.size(); i++) 
     std::cout<< graphicPixels[i].x<<std::endl;
     
 }
 
 void clearPixels() 
 {
     graphicPixels.clear();
 }
 
 void createConsole() 
 {
        
        ImGui::SetNextWindowPos(ImVec2(820, 570), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(1030, 430), ImGuiCond_None);
        
        ImGui::Begin("Console (F2 - Taste)", NULL, FLAGS_CONSOLE);
          
        
        ImGui::End();
 }

int count = 100;





// Main code
int main(int, char**)
{
    
    // Smiley demo
    drawPixel(50, 50, 255, 0,0, 255, 15);
    drawPixel(200, 50, 255, 0,0, 255, 15);
    drawPixel(50, 150, 255, 0,0, 255, 15);
    drawPixel(80, 150, 255, 0,0, 255, 15);
    drawPixel(110, 150, 255, 0,0, 255, 15);
    drawPixel(140, 150, 255, 0,0, 255, 15);
    drawPixel(170, 150, 255, 0,0, 255, 15);
    drawPixel(200, 150, 255, 0,0, 255, 15);
    drawPixel(135, 75, 255, 0,0, 255, 15);
    
    
    // Setup SDL
    // (Some versions of SDL before <2.0.10 appears to have performance/stalling issues on a minority of Windows systems,
    // depending on whether SDL_INIT_GAMECONTROLLER is enabled or disabled.. updating to latest version of SDL is recommended!)
    if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_TIMER | SDL_INIT_GAMECONTROLLER) != 0)
    {
        printf("Error: %s\n", SDL_GetError());
        return -1;
    }

    // Setup window
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 2);
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE | SDL_WINDOW_ALLOW_HIGHDPI);
    SDL_Window* window = SDL_CreateWindow("Dear ImGui SDL2+OpenGL example", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, window_flags);
    SDL_GLContext gl_context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, gl_context);
    SDL_GL_SetSwapInterval(1); // Enable vsync
    SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);


    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
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

    // Our state
   // bool show_app_console = true;
    bool show_demo_window = true;
    
    
    ImVec4 clear_color = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

    // Main loop
    bool done = false;
    while (!done)
    {
        
        count--;
        if (count == 0) {
            clearPixels();
        }
        
        // Poll and handle events (inputs, window resize, etc.)
        // You can read the io.WantCaptureMouse, io.WantCaptureKeyboard flags to tell if dear imgui wants to use your inputs.
        // - When io.WantCaptureMouse is true, do not dispatch mouse input data to your main application.
        // - When io.WantCaptureKeyboard is true, do not dispatch keyboard input data to your main application.
        // Generally you may always pass all inputs to dear imgui, and hide them from your application based on those two flags.
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            ImGui_ImplSDL2_ProcessEvent(&event);
            if (event.type == SDL_QUIT)
                done = true;
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(window))
                done = true;
        }

        // Start the Dear ImGui frame
        ImGui_ImplOpenGL2_NewFrame();
        ImGui_ImplSDL2_NewFrame(window);
        ImGui::NewFrame();

       

        // 1. Show a simple window that we create ourselves. We use a Begin/End pair to created a named window.
        
        ImGui::ShowDemoWindow(&show_demo_window); 
        
        // Create the needed windows
        createMainMenuBar();
        createEditor();
        createHelpwindow();
        createGraphicalOutput();
        createConsole();
        
        
       

        // Rendering
        ImGui::Render();
        glViewport(0, 0, (int)io.DisplaySize.x, (int)io.DisplaySize.y);
        glClearColor(clear_color.x, clear_color.y, clear_color.z, clear_color.w);
        glClear(GL_COLOR_BUFFER_BIT);
        //glUseProgram(0); // You may want this if using this code in an OpenGL 3+ context where shaders may be bound
        ImGui_ImplOpenGL2_RenderDrawData(ImGui::GetDrawData());
        SDL_GL_SwapWindow(window);
    }

    // Cleanup
    ImGui_ImplOpenGL2_Shutdown();
    ImGui_ImplSDL2_Shutdown();
    ImGui::DestroyContext();

    SDL_GL_DeleteContext(gl_context);
    SDL_DestroyWindow(window);
    SDL_Quit();

    return 0;
}
