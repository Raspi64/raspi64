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

private:
    lua_State *L;

    void load_libraries();

    void replace_function_in_table(const char *table, const char *field, lua_CFunction function);

    static int lua_function_not_allowed(lua_State *state);

    static int lua_print(lua_State *state);

    static int lua_draw(lua_State *state);

    static int lua_clear(lua_State *state);

    static int lua_error_handler(lua_State *L);

    static void parse_error_message(std::string &error_message);
};


#endif //IMGUI_SETUP_LUAPLUGIN_HPP
