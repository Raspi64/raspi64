//
// Created by simon on 12/1/20.
//

#include <iostream>
#include <unistd.h>
#include <string>
#include "Plugin.hpp"
#include "LuaPlugin.hpp"

LuaPlugin::LuaPlugin() : Plugin() {
    L = luaL_newstate();

    load_libraries();

    // register custom functions
    lua_register(L, "print", lua_print);
    lua_register(L, "draw", lua_draw);
    lua_register(L, "clear", lua_clear);
    lua_register(L, "sleep", lua_sleep);

    // overwrite unwanted functions
    lua_register(L, "collectgarbage", lua_function_not_allowed);
    lua_register(L, "dofile", lua_function_not_allowed);
    lua_register(L, "load", lua_function_not_allowed);
    lua_register(L, "loadfile", lua_function_not_allowed);
    lua_register(L, "require", lua_function_not_allowed);
    replace_function_in_table("io", "read", lua_function_not_allowed);
    replace_function_in_table("io", "write", lua_function_not_allowed);
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

std::string LuaPlugin::get_extension() {
    return ".lua";
}

std::string LuaPlugin::get_help_folder_name() {
    return "Lua";
}

void LuaPlugin::load_libraries() {
    const luaL_Reg loadedlibs[] = {
            {LUA_GNAME,       luaopen_base},
//            {LUA_LOADLIBNAME, luaopen_package},
            {LUA_COLIBNAME,   luaopen_coroutine},
            {LUA_TABLIBNAME,  luaopen_table},
            {LUA_IOLIBNAME,   luaopen_io},
            {LUA_OSLIBNAME,   luaopen_os},
            {LUA_STRLIBNAME,  luaopen_string},
            {LUA_MATHLIBNAME, luaopen_math},
//            {LUA_UTF8LIBNAME, luaopen_utf8},
//            {LUA_DBLIBNAME, luaopen_debug},
            {nullptr,         nullptr}
    };

    const luaL_Reg *lib;
    /* "require" functions from 'loadedlibs' and set results to global table */
    for (lib = loadedlibs; lib->func; lib++) {
        luaL_requiref(L, lib->name, lib->func, 1);
        lua_pop(L, 1);  /* remove lib */
    }
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

int LuaPlugin::lua_draw(lua_State *state) {
    if (lua_gettop(state) != 7) {
        return luaL_error(state, "expecting parameters: x,y,r,g,b,a,size");
    }
    for (int i = 1; i <= 7; ++i) {
        if (!lua_isnumber(state, i)) {
            return luaL_error(state, "expecting only numbers as parameters");
        }
    }
    int x = lua_tointeger(state, 1);
    int y = lua_tointeger(state, 2);
    int red = lua_tointeger(state, 3);
    int green = lua_tointeger(state, 4);
    int blue = lua_tointeger(state, 5);
    int alpha = lua_tointeger(state, 6);
    int size = lua_tointeger(state, 7);

    Plugin::draw(x, y, red, green, blue, alpha, size);

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
    on_error(stoi(str), error_message);
}
