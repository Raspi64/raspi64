//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_GRAPHICWINDOW_HPP
#define IMGUI_SETUP_GRAPHICWINDOW_HPP

#include <vector>

#include "base/Window.hpp"
#include "../../types.hpp"


class GraphicWindow: public Window {
public:
    GraphicWindow();
    void render();

    void add_pixel(TGraphicPixel pixel);
    void add_rect(TGraphicRect rect);
    void add_circle(TGraphicCircle circle);
    void add_line(TGraphicLine line);
    void add_text(TGraphicText text);
    void clear();

private:
    std::vector<TGraphicDrawEntity> graphicDrawEntities;
    void draw_graphic_entities(ImVec2 origin, ImDrawList *draw_list);
};


#endif //IMGUI_SETUP_GRAPHICWINDOW_HPP
