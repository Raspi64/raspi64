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

    void add_pixel(int x, int y, int red, int green, int blue, int alpha = 255, int size = 1);
    void clear_pixels();

private:
    std::vector<TGraphicPixel> graphicPixels;
    void draw_graphic_pixels(ImVec2 origin, ImDrawList *draw_list);
};


#endif //IMGUI_SETUP_GRAPHICWINDOW_HPP
