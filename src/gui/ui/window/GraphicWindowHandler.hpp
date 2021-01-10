//
// Created by Alex on 15.11.20.
//

#ifndef IMGUI_SETUP_GRAPHICWINDOWHANDLER_HPP
#define IMGUI_SETUP_GRAPHICWINDOWHANDLER_HPP

#include "imgui.h"

#include "base/WindowHandler.hpp"
#include "GraphicWindow.hpp"


class GraphicWindowHandler: public WindowHandler {
public:
    GraphicWindowHandler(GraphicWindow* window);
    void add_pixel(TGraphicPixel pixel);
    void add_rect(TGraphicRect rect);
    void add_circle(TGraphicCircle circle);
    void add_line(TGraphicLine line);
    void add_text(TGraphicText text);
    void add_pixel(int x, int y, int red, int green, int blue, int alpha = 255, int size = 1);
    void clear();
private:
    GraphicWindow* window;
};


#endif //IMGUI_SETUP_GRAPHICWINDOWHANDLER_HPP
