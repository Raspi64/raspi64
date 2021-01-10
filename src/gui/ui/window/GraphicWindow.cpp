//
// Created by Alex on 15.11.20.
//

#include <cmath>
#include "imgui.h"

#include "GraphicWindow.hpp"
#include "gui/config.hpp"

GraphicWindow::GraphicWindow() : Window() {
    clear();
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

    draw_graphic_entities(origin, draw_list);

    draw_list->PopClipRect();

    ImGui::End();
}

void GraphicWindow::draw_graphic_entities(ImVec2 origin, ImDrawList *draw_list) {
    /*
    for (size_t i = 0; i < graphicPixels.size(); i++) {
        TGraphicPixel t = graphicPixels[i];
        int offset = (int) t.size / 2;
        draw_list->AddRectFilled(ImVec2(origin.x + t.x - offset, origin.y + t.y - offset),
                                 ImVec2(origin.x + t.x + std::max(1, offset), origin.y + t.y + std::max(1, offset)),
                                 IM_COL32(t.color.red, t.color.green, t.color.blue, t.color.alpha));
    }
    */

    for (auto t: graphicDrawEntities) {
        auto color = IM_COL32(t.entity->color.red, t.entity->color.green, t.entity->color.blue, t.entity->color.alpha);

        switch (t.type) {
            case TYPE_RECT: {
                auto entity = (TGraphicRect *) t.entity;

                auto start = ImVec2(origin.x + entity->start_x, origin.y + entity->start_y);
                auto end = ImVec2(origin.x + entity->end_x, origin.y + entity->end_y);

                if (entity->filled)
                    draw_list->AddRectFilled(start, end, color);
                else
                    draw_list->AddRect(start, end, color, 0.0f, ImDrawCornerFlags_All, entity->thickness);
                break;
            }
            case TYPE_CIRCLE: {
                auto entity = (TGraphicCircle *) t.entity;

                auto pos = ImVec2(origin.x + entity->x, origin.y + entity->y);

                if (entity->filled)
                    draw_list->AddCircleFilled(pos, entity->radius, color);
                else
                    draw_list->AddCircle(pos, entity->radius, color, 0, entity->thickness);
                break;
            }
            case TYPE_LINE: {
                auto entity = (TGraphicLine *) t.entity;

                draw_list->AddLine(ImVec2(origin.x + entity->start_x, origin.y + entity->start_y),
                                   ImVec2(origin.x + entity->end_x, origin.y + entity->end_y),
                                   color, entity->thickness);

                break;
            }
            case TYPE_TEXT: {
                auto entity = (TGraphicText *) t.entity;

                draw_list->AddText(NULL,
                                   entity->size,
                                   ImVec2(origin.x + entity->x, origin.y + entity->y),
                                   color,
                                   entity->text);
            }
        }
    }
}

void GraphicWindow::add_rect(TGraphicRect rect) {
    graphicDrawEntities.push_back({new TGraphicRect(rect), TYPE_RECT});
}

void GraphicWindow::add_circle(TGraphicCircle circle) {
    graphicDrawEntities.push_back({new TGraphicCircle(circle), TYPE_CIRCLE});
}

void GraphicWindow::add_line(TGraphicLine line) {
    graphicDrawEntities.push_back({new TGraphicLine(line), TYPE_LINE});
}

void GraphicWindow::add_text(TGraphicText text) {
    graphicDrawEntities.push_back({new TGraphicText(text), TYPE_TEXT});
}

void GraphicWindow::add_pixel(TGraphicPixel pixel) {
    int offset = (int) pixel.size / 2;
    TGraphicRect rect{};
    rect.start_x = pixel.x - offset;
    rect.start_y = pixel.y - offset;
    rect.end_x = pixel.x + std::max(1, offset);
    rect.end_y = pixel.y + std::max(1, offset);
    rect.color = pixel.color;
    rect.filled = true;
    add_rect(rect);
}

void GraphicWindow::clear() {
    graphicDrawEntities.clear();
}
