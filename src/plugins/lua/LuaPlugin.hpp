//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_LUAPLUGIN_HPP
#define IMGUI_SETUP_LUAPLUGIN_HPP


#include <Plugin.hpp>
#include <lua.hpp>

class LuaPlugin : public Plugin {
public:
    LuaPlugin(
            draw_funct_t draw_function_value,
            clear_funct_t clear_function_value,
            print_funct_t print_function_value
    );

    ~LuaPlugin() override;

    bool load_script(const std::string &lua_script) override;

    bool exec_script() override;

};


#endif //IMGUI_SETUP_LUAPLUGIN_HPP
