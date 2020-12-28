//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_PLUGIN_HPP
#define IMGUI_SETUP_PLUGIN_HPP


#include <string>

class Plugin {
public:

    Plugin() = default;

    virtual ~Plugin() = default;

    virtual bool load_script(const std::string &) = 0;

    virtual bool exec_script() = 0;

	virtual std::string get_extension() = 0;

	virtual std::string get_help_folder_name() = 0;

    static void print(const std::string &message);

    static void draw(int x, int y, int red, int green, int blue, int alpha, int size);

    static void clear();

    static void on_error(int line, const std::string &message);
};


#endif //IMGUI_SETUP_PLUGIN_HPP
