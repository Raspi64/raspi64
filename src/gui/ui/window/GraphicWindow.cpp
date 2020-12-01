//
// Created by Alex on 15.11.20.
//

#include <cmath>
#include "imgui.h"

#include "GraphicWindow.hpp"
#include "gui/config.hpp"

GraphicWindow::GraphicWindow(): Window() {
    graphicPixels.clear();
}

void GraphicWindow::render() {
    ImGui::SetNextWindowPos(ImVec2(1350, 50), ImGuiCond_None);
    ImGui::SetNextWindowSize(ImVec2(500, 500), ImGuiCond_None);

    ImGui::Begin(WIN_TITLE_GRAPHIC, NULL, FLAGS_CANVAS);
    after_imgui_begin(WIN_TITLE_GRAPHIC);


    static ImVec2 scrolling(0.0f, 0.0f);
    static bool opt_enable_grid = true;


    ImVec2 canvas_p0 = ImGui::GetCursorScreenPos(); // ImDrawList API uses screen coordinates!
    ImVec2 canvas_sz = ImGui::GetContentRegionAvail(); // Resize canvas to what's available
    if (canvas_sz.x < 50.0f) canvas_sz.x = 50.0f;
    if (canvas_sz.y < 50.0f) canvas_sz.y = 50.0f;
    ImVec2 canvas_p1 = ImVec2(canvas_p0.x + canvas_sz.x, canvas_p0.y + canvas_sz.y);

    // Draw border and background color
    ImDrawList *draw_list = ImGui::GetWindowDrawList();
    draw_list->AddRectFilled(canvas_p0, canvas_p1, IM_COL32(50, 50, 50, 255));
    draw_list->AddRect(canvas_p0, canvas_p1, IM_COL32(255, 255, 255, 255));

    // This will catch our interactions
    ImGui::InvisibleButton("canvas", canvas_sz, ImGuiButtonFlags_MouseButtonLeft | ImGuiButtonFlags_MouseButtonRight);
    const ImVec2 origin(canvas_p0.x + scrolling.x, canvas_p0.y + scrolling.y);



    // Draw grid + all lines in the canvas
    draw_list->PushClipRect(canvas_p0, canvas_p1, true);
    if (opt_enable_grid) {
        const float GRID_STEP = 16.0f;
        for (float x = fmodf(scrolling.x, GRID_STEP); x < canvas_sz.x; x += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x + x, canvas_p0.y), ImVec2(canvas_p0.x + x, canvas_p1.y),
                               IM_COL32(200, 200, 200, 40));
        for (float y = fmodf(scrolling.y, GRID_STEP); y < canvas_sz.y; y += GRID_STEP)
            draw_list->AddLine(ImVec2(canvas_p0.x, canvas_p0.y + y), ImVec2(canvas_p1.x, canvas_p0.y + y),
                               IM_COL32(200, 200, 200, 40));
    }

    draw_graphic_pixels(origin, draw_list);

    draw_list->PopClipRect();

    ImGui::End();
}

void GraphicWindow::draw_graphic_pixels(ImVec2 origin, ImDrawList *draw_list) {
    for (size_t i = 0; i < graphicPixels.size(); i++) {
        TGraphicPixel t = graphicPixels[i];
        int offset = (int) t.size / 2;
        draw_list->AddRectFilled(ImVec2(origin.x + t.x - offset, origin.y + t.y - offset),
                                 ImVec2(origin.x + t.x + std::max(1, offset), origin.y + t.y + std::max(1, offset)),
                                 IM_COL32(t.red, t.green, t.blue, t.alpha));
    }
}

void GraphicWindow::add_pixel(int x, int y, int red, int green, int blue, int alpha, int size) {
    TGraphicPixel test;
    test.size = size;
    test.x = x;
    test.y = y;
    test.red = red;
    test.green = green;
    test.blue = blue;
    test.alpha = alpha;

    graphicPixels.push_back(test);
}

void GraphicWindow::clear_pixels() {
    graphicPixels.clear();
}
