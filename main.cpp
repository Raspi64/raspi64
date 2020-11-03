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
#include <SDL.h>
#include <SDL_opengl.h>
#define FLAGS_BASE (0 | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoScrollbar)
#define FLAGS_EDITOR (FLAGS_BASE)
#define FLAGS_HELP (FLAGS_BASE)
#define FLAGS_CANVAS (FLAGS_BASE)
#define FLAGS_CONSOLE (FLAGS_BASE)

// Main code
int main(int, char**)
{
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
        
        if (ImGui::BeginMainMenuBar())
        {

            ImGui::Text("HELP - Hilfe anzeigen ");

            ImGui::Text("CRTL + C - Programm beenden ");

            ImGui::Text("RUN - Programm starten ");


            ImGui::EndMainMenuBar();
        }
            
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
        
             
        ImGui::SetNextWindowPos(ImVec2(820, 50), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);
        
        //Help window
        ImGui::Begin("Help Window (F3 - Taste)", NULL, FLAGS_HELP);
        static char buf[32] = "\xe6\x97\xa5\xe6\x9c\xac\xe8\xaa\x9e"; 
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

        // 2. Show another simple window.s
        
        ImGui::SetNextWindowPos(ImVec2(1350, 50), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);
    
        ImGui::Begin("Graphical Output (F4 - Taste)", NULL, FLAGS_CANVAS);   
        
        
        static ImVector<ImVec2> points;
        static ImVec2 scrolling(0.0f, 0.0f);
        static bool opt_enable_grid = true;
        static bool opt_enable_context_menu = true;
        static bool adding_line = false;
        
        ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
        ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
        if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
        if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
        ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);
        
        // Draw border and background color
        ImGuiIO& io = ImGui::GetIO();
        ImDrawList* draw_list = ImGui::GetWindowDrawList();
        draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
        draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));
        
        // This will catch our interactions
        ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
        const bool is_hovered = ImGui::IsItemHovered();
        const bool is_active = ImGui::IsItemActive();
        const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);
        const ImVec2 mouse_pos_in_canvas(io.MousePos.x- origin.x, io.MousePos.y - origin.y);
        
        // Add first and second point
        if(is_hovered && !adding_line && ImGui::IsMouseClicked(ImGuiMouseButton_Left))
        {
            points.push_back(mouse_pos_in_canvas);
            points.push_back(mouse_pos_in_canvas);
            adding_line = true;
        }
        if(adding_line)
        {
            points.back() = mouse_pos_in_canvas;
            if(!ImGui::IsMouseDown(ImGuiMouseButton_Left))
                adding_line = false;
        }
        
        // Pan (we use a zero mouse threshold when there's no context menu)
        // You may decide to make that threshold dynmaic based on wether th mouse is hovering something etc.
        const float mouse_threshold_for_pan =opt_enable_context_menu ? -1.0f : 0.0f;
        if (is_active && ImGui::IsMouseDragging(ImGuiMouseButton_Right, mouse_threshold_for_pan))
        {
            scrolling.x += io.MouseDelta.x;
            scrolling.y += io.MouseDelta.y;
        }
        
        // Context menu (under default mouse threshold)
        ImVec2 drag_delta = ImGui::GetMouseDragDelta(ImGuiMouseButton_Right);
        if (opt_enable_context_menu && ImGui::IsMouseReleased(ImGuiMouseButton_Right) && drag_delta.x == 0.0f && drag_delta.y == 0.0f)
            ImGui::OpenPopupOnItemClick("context");
        if (ImGui::BeginPopup("context"))
        {
            if (adding_line) 
                points.resize(points.size() - 2);
            adding_line = false;
            if (ImGui::MenuItem("Remove one", NULL, false, points.Size > 0)) { points.resize(points.size() - 2); }
            if (ImGui::MenuItem("Remove all", NULL, false, points.Size > 0)) { points.clear(); }  
            ImGui::EndPopup();
        }
        
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
        for (int n = 0; n < points.Size; n += 2)
            draw_list->AddLine(ImVec2(origin.x + points[n].x, origin.y + points[n].y), ImVec2(origin.x + points[n + 1].x, origin.y + points[n + 1].y), IM_COL32(255, 255, 0, 255), 2.0f);
        draw_list->PopClipRect();
        
        
        
        
        /*ImGuiIO& io = ImGui::GetIO();
        ImTextureID my_tex_id = io.Fonts->TexID;
        float my_tex_w = (float)io.Fonts->TexWidth;
        float my_tex_h = (float)io.Fonts->TexHeight;
        {
        ImGui::Text("%.0fx%.0f", my_tex_w, my_tex_h);
        //ImVec2 pos = ImGui::GetCursorScreenPos();
        ImVec2 uv_min = ImVec2(0.0f, 0.0f); // Top-Left
        ImVec2 uv_max = ImVec2(1.0f, 1.0f); // lower right
        ImVec4 tint_col = ImVec4(1.0f, 1.0f, 1.0f, 1.0f); // No tint
        ImVec4 border_col = ImVec4(1.0f, 1.0f, 1.0f, 0.5f); // 50% opaque white      
        ImGui::Image(my_tex_id, ImVec2(my_tex_w, my_tex_h), uv_min, uv_max, tint_col, border_col);
        } */  
        ImGui::End();
        
        
        // 3. Show window number 3.
        
        ImGui::SetNextWindowPos(ImVec2(820, 570), ImGuiCond_None);
        ImGui::SetNextWindowSize(ImVec2(1030, 430), ImGuiCond_None);
        
        ImGui::Begin("Console (F2 - Taste)", NULL, FLAGS_CONSOLE);
          
        
        ImGui::End();

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
