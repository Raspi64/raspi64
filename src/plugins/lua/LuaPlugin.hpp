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

private:
    lua_State *L;

    static void set_error_message(const std::string &error_message);

    bool load_script(const std::string &lua_script) override;

    bool exec_script() override;

    static void replace_function_in_table(lua_State *L, const char *table, const char *field, lua_CFunction function);

    static int lua_error_handler(lua_State *L);

    static int lua_draw(lua_State *state);

    static int lua_clear(lua_State *state);

    static int lua_print(lua_State *state);
};


#endif //IMGUI_SETUP_LUAPLUGIN_HPP
