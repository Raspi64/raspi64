//
// Created by simon on 12/1/20.
//

#include "Plugin.hpp"

Plugin::Plugin(draw_funct_t draw_function_value, clear_funct_t clear_function_value, print_funct_t print_function_value) {
    Plugin::draw_function = draw_function_value;
    Plugin::clear_function = clear_function_value;
    Plugin::print_function = print_function_value;
}

draw_funct_t Plugin::draw_function;
clear_funct_t Plugin::clear_function;
print_funct_t Plugin::print_function;
