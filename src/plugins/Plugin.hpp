//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_PLUGIN_HPP
#define IMGUI_SETUP_PLUGIN_HPP


#include <string>
#include <HelpSystem.hpp>

class Plugin {
public:

    Plugin() = default;

    virtual ~Plugin() = default;

    virtual bool load_script(const std::string &) = 0;

    virtual bool exec_script() = 0;

    virtual void on_key_press(const std::string &) = 0;

    virtual void on_key_release(const std::string &) = 0;

    virtual std::string get_extension() = 0;

    virtual std::string get_help_folder_name() = 0;

    static void print(const std::string &message);

    static void draw_rect(TGraphicRect rect);

    static void draw_line(TGraphicLine line);

    static void draw_text(TGraphicText text);

    static void draw_pixel(TGraphicPixel pixel);

    static void draw_circle(TGraphicCircle circle);

    static void clear();

    static std::string get_input_line();

    static void on_error(int line, const std::string &message);
};


#endif //IMGUI_SETUP_PLUGIN_HPP
