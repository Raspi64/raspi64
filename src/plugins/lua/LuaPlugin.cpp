//
// Created by simon on 12/1/20.
//

#include <unistd.h>
#include <string>
#include <stdexcept>
#include "Plugin.hpp"
#include "LuaPlugin.hpp"

LuaPlugin::LuaPlugin() : Plugin() {
    L = luaL_newstate();

    luaL_openlibs(L);

    // register custom functions
    lua_register(L, "print", lua_print);
    lua_register(L, "draw", lua_draw_pixel);
    lua_register(L, "draw_pixel", lua_draw_pixel);
    lua_register(L, "draw_circle", lua_draw_circle);
    lua_register(L, "draw_line", lua_draw_line);
    lua_register(L, "draw_rect", lua_draw_rect);
    lua_register(L, "draw_text", lua_draw_text);
    lua_register(L, "clear", lua_clear);
    lua_register(L, "sleep", lua_sleep);

    // overwrite unwanted functions
    lua_register(L, "collectgarbage", lua_function_not_allowed);
    lua_register(L, "dofile", lua_function_not_allowed);
    lua_register(L, "load", lua_function_not_allowed);
    lua_register(L, "loadfile", lua_function_not_allowed);
    lua_register(L, "require", lua_function_not_allowed);

    // create io table
    lua_newtable(L);
    lua_setglobal(L, "io");
    replace_function_in_table("io", "read", lua_io_read);
    replace_function_in_table("io", "write", lua_io_write);
}

LuaPlugin::~LuaPlugin() {
    lua_close(L);
}

bool LuaPlugin::load_script(const std::string &lua_script) {
    int load_stat = luaL_loadbuffer(L, lua_script.c_str(), lua_script.length(), lua_script.c_str());

    if (load_stat != LUA_OK) {
        std::string message = std::string(lua_tostring(L, -1));
        parse_error_message(message);
    }
    return load_stat == LUA_OK;
}

bool LuaPlugin::exec_script() {
    /* calculate stack position for message handler */
    int hpos = lua_gettop(L);
    /* push custom error message handler */
    lua_pushcfunction(L, lua_error_handler);
    /* move it before function and arguments */
    lua_insert(L, hpos);
    /* call lua_pcall function with custom handler */
    int exec_stat = lua_pcall(L, 0, 0, hpos);
    /* remove custom error message handler from stack */
    lua_remove(L, hpos);

    return exec_stat == LUA_OK;
}

void LuaPlugin::on_key_press(const std::string &key) {
    int type = lua_getglobal(L, "on_key_press");
    if (type == LUA_TFUNCTION) {
        lua_pushstring(L, key.c_str()); // 1st argument
        lua_call(L, 1, 0);     // call 'on_key_press' with 1 arguments and 0 results
    } else {
        lua_pop(L, 1);
    }
}

void LuaPlugin::on_key_release(const std::string &key) {
    int type = lua_getglobal(L, "on_key_release");
    if (type == LUA_TFUNCTION) {
        lua_pushstring(L, key.c_str()); // 1st argument
        lua_call(L, 1, 0);     // call 'on_key_release' with 1 arguments and 0 results
    } else {
        lua_pop(L, 1);
    }
}

std::string LuaPlugin::get_extension() {
    return ".lua";
}

std::string LuaPlugin::get_help_folder_name() {
    return "Lua";
}

void LuaPlugin::replace_function_in_table(const char *table, const char *field, lua_CFunction function) {
    lua_getglobal(L, table);
    lua_pushstring(L, field);
    lua_pushcfunction(L, function);
    lua_settable(L, -3);
}

int LuaPlugin::lua_function_not_allowed(lua_State *state) {
    return luaL_error(state, "this function is not allowed");
}

int LuaPlugin::lua_io_read(lua_State *state) {
    if (lua_gettop(state) != 0) {
        return luaL_error(state, "This function has been simplified to always return a full line of input. No arguments possible.");
    }
    lua_pushstring(state, Plugin::get_input_line().c_str());
    return 1;
}

int LuaPlugin::lua_io_write(lua_State *state) {
    lua_print(state);
    return 0;
}

int LuaPlugin::lua_print(lua_State *state) {
    std::string return_string;

    int top = lua_gettop(state);
    for (int i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(state, i);
        double number;

        switch (t) {
            case LUA_TSTRING:  /* strings */
                return_string += lua_tostring(state, i);
                break;

            case LUA_TBOOLEAN:  /* booleans */
                return_string += lua_toboolean(state, i) ? "true" : "false";
                break;

            case LUA_TNUMBER:  /* numbers */
                number = lua_tonumber(state, i);
                if ((double) ((int) number) == number) { // if is integer
                    return_string += std::to_string((int) number);
                } else {
                    return_string += std::to_string(number);
                    return_string.erase(return_string.find_last_not_of('0') + 1, std::string::npos);
                }
                break;

            default:  /* other values */
                return_string += lua_typename(state, t);
                break;

        }

        if (i < top) {
            return_string += '\t';
        }
    }

    Plugin::print(return_string);

    return 0;
}

TColor LuaPlugin::get_color(lua_State *state, int color_table_pos) {
    TColor color{};
    int old_top = lua_gettop(state);

    int *colors[4] = {&color.red, &color.green, &color.blue, &color.alpha};
    char *color_names[4] = {"red", "green", "blue", "alpha"};

    for (int i = 0; i < 4; i++) {
        // get field from lua
        lua_getfield(state, color_table_pos, color_names[i]);
        luaL_checktype(state, lua_gettop(state), LUA_TNUMBER);
        *colors[i] = (int) lua_tonumber(state, lua_gettop(state));

        // sanity check
        if (*colors[i] > 255) {
            *colors[i] = 255;
        } else if (*colors[i] < 0) {
            *colors[i] = 0;
        }
    }

    lua_settop(state, old_top);
    return color;
}

int LuaPlugin::lua_draw_rect(lua_State *state) {
    TGraphicRect rect{};

    if (lua_gettop(state) != 7) {
        return luaL_error(state, "Erwarte Parameter: start_x,start_y,end_x,end_y,thickness,filled,color (Table color hat Felder: red,green,blue,alpha)");
    }

    luaL_checktype(state, 1, LUA_TNUMBER);
    rect.start_x = (int) lua_tonumber(state, 1);

    luaL_checktype(state, 2, LUA_TNUMBER);
    rect.start_y = (int) lua_tonumber(state, 2);

    luaL_checktype(state, 3, LUA_TNUMBER);
    rect.end_x = (int) lua_tonumber(state, 3);

    luaL_checktype(state, 4, LUA_TNUMBER);
    rect.end_y = (int) lua_tonumber(state, 4);

    luaL_checktype(state, 5, LUA_TNUMBER);
    rect.thickness = lua_tonumber(state, 5);

    luaL_checktype(state, 6, LUA_TBOOLEAN);
    rect.filled = lua_toboolean(state, 6);

    luaL_checktype(state, 7, LUA_TTABLE);
    rect.color = get_color(state, 7);

    Plugin::draw_rect(rect);
    return 0;
}

int LuaPlugin::lua_draw_line(lua_State *state) {
    TGraphicLine line{};

    if (lua_gettop(state) != 6) {
        return luaL_error(state, "Erwarte Parameter: start_x,start_y,end_x,end_y,thickness,color (Table color hat Felder: red,green,blue,alpha)");
    }

    luaL_checktype(state, 1, LUA_TNUMBER);
    line.start_x = (int) lua_tonumber(state, 1);

    luaL_checktype(state, 2, LUA_TNUMBER);
    line.start_y = (int) lua_tonumber(state, 2);

    luaL_checktype(state, 3, LUA_TNUMBER);
    line.end_x = (int) lua_tonumber(state, 3);

    luaL_checktype(state, 4, LUA_TNUMBER);
    line.end_y = (int) lua_tonumber(state, 4);

    luaL_checktype(state, 5, LUA_TNUMBER);
    line.thickness = lua_tonumber(state, 5);

    luaL_checktype(state, 6, LUA_TTABLE);
    line.color = get_color(state, 6);

    Plugin::draw_line(line);
    return 0;
}

int LuaPlugin::lua_draw_text(lua_State *state) {
    TGraphicText text{};

    if (lua_gettop(state) != 5) {
        return luaL_error(state, "Erwarte Parameter: x,y,size,text,color (Table color hat Felder: red,green,blue,alpha)");
    }

    luaL_checktype(state, 1, LUA_TNUMBER);
    text.x = (int) lua_tonumber(state, 1);

    luaL_checktype(state, 2, LUA_TNUMBER);
    text.y = (int) lua_tonumber(state, 2);

    luaL_checktype(state, 3, LUA_TNUMBER);
    text.size = (float) lua_tonumber(state, 3);

    luaL_checktype(state, 4, LUA_TSTRING);
    asprintf(&text.text, "%s", lua_tostring(state, 4));

    luaL_checktype(state, 5, LUA_TTABLE);
    text.color = get_color(state, 5);

    Plugin::draw_text(text);
    return 0;
}

int LuaPlugin::lua_draw_pixel(lua_State *state) {
    TGraphicPixel pixel{};
    if (lua_gettop(state) != 4) {
        return luaL_error(state, "Erwarte Parameter: x,y,size,color (Table color hat Felder: red,green,blue,alpha)");
    }

    luaL_checktype(state, 1, LUA_TNUMBER);
    pixel.x = (int) lua_tonumber(state, 1);

    luaL_checktype(state, 2, LUA_TNUMBER);
    pixel.y = (int) lua_tonumber(state, 2);

    luaL_checktype(state, 3, LUA_TNUMBER);
    pixel.size = (int) lua_tonumber(state, 3);

    luaL_checktype(state, 4, LUA_TTABLE);
    pixel.color = get_color(state, 4);

    Plugin::draw_pixel(pixel);
    return 0;
}

int LuaPlugin::lua_draw_circle(lua_State *state) {
    TGraphicCircle circle{};

    if (lua_gettop(state) != 4) {
        return luaL_error(state, "Erwarte Parameter: x,y,radius,color (Table color hat Felder: red,green,blue,alpha)");
    }

    luaL_checktype(state, 1, LUA_TNUMBER);
    circle.x = (int) lua_tonumber(state, 1);

    luaL_checktype(state, 2, LUA_TNUMBER);
    circle.y = (int) lua_tonumber(state, 2);

    luaL_checktype(state, 3, LUA_TNUMBER);
    circle.radius = lua_tonumber(state, 3);

    luaL_checktype(state, 4, LUA_TTABLE);
    circle.color = get_color(state, 4);

    Plugin::draw_circle(circle);
    return 0;
}

int LuaPlugin::lua_clear(lua_State *state) {
    if (lua_gettop(state) != 0) {
        return luaL_error(state, "expecting no parameters");
    }

    Plugin::clear();

    return 0;
}

int LuaPlugin::lua_sleep(lua_State *state) {
    if (lua_gettop(state) != 1) {
        return luaL_error(state, "expecting parameters: time in seconds");
    }
    if (!lua_isnumber(state, 1)) {
        return luaL_error(state, "expecting a number of seconds as parameter");
    }
    lua_Number time = lua_tonumber(state, 1);
    if (time < 0) {
        return luaL_error(state, "can't sleep for a negative amount of time");
    }
    double utime = time * 1000000;

    usleep((unsigned int) utime);

    return 0;
}

int LuaPlugin::lua_error_handler(lua_State *L) {
    std::string error_message = std::string(lua_tostring(L, -1));
    lua_remove(L, -1);
    parse_error_message(error_message);
    return 0;
}

void LuaPlugin::parse_error_message(std::string &error_message) {
    unsigned long start = error_message.find(':') + 1;
    unsigned long end = error_message.find(':', start);
    std::string str = error_message.substr(start, end - start);
    printf("Error: '%s' '%s'\n", str.c_str(), error_message.c_str());
    try {
        int line = stoi(str);
        on_error(line, error_message);
    } catch (std::invalid_argument &argument) {}
}
