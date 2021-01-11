//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_LUAPLUGIN_HPP
#define IMGUI_SETUP_LUAPLUGIN_HPP


#include <Plugin.hpp>
#include <lua.hpp>

class LuaPlugin : public Plugin {
public:
    LuaPlugin();

    ~LuaPlugin() override;

    bool load_script(const std::string &lua_script) override;

    bool exec_script() override;

    void on_key_press(const std::string &) override;

    void on_key_release(const std::string &) override;

    std::string get_extension() override;

    std::string get_help_folder_name() override;

private:
    lua_State *L;

    void replace_function_in_table(const char *table, const char *field, lua_CFunction function);

    static int lua_function_not_allowed(lua_State *state);

    static int lua_print(lua_State *state);

    static int lua_draw_pixel(lua_State *state);

    static int lua_draw_rect(lua_State *state);

    static int lua_draw_line(lua_State *state);

    static int lua_draw_text(lua_State *state);

    static int lua_draw_circle(lua_State *state);

    static int lua_clear(lua_State *state);

    static int lua_error_handler(lua_State *L);

    static void parse_error_message(std::string &error_message);

    static int lua_sleep(lua_State *state);

    static int lua_io_read(lua_State *state);

    static int lua_io_write(lua_State *state);

    static TColor get_color(lua_State *state);

    static TColor get_color(lua_State *state, int color_table_pos);
};


#endif //IMGUI_SETUP_LUAPLUGIN_HPP
