//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_PLUGIN_HPP
#define IMGUI_SETUP_PLUGIN_HPP


#include <string>
#include <types.hpp>

class Plugin {
public:

    Plugin(
            draw_funct_t draw_function_value,
            clear_funct_t clear_function_value,
            print_funct_t print_function_value
    );

    virtual ~Plugin() = default;

    virtual bool load_script(const std::string &) = 0;

    virtual bool exec_script() = 0;

    static std::string last_error_buffer;
    static int last_error_line;
protected:
    static draw_funct_t draw_function;
    static clear_funct_t clear_function;
    static print_funct_t print_function;
};


#endif //IMGUI_SETUP_PLUGIN_HPP
