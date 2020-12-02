//
// Created by simon on 12/1/20.
//

#include <iostream>
#include "Plugin.hpp"
#include "LuaPlugin.hpp"

LuaPlugin::LuaPlugin(draw_funct_t draw_function_value, clear_funct_t clear_function_value, print_funct_t print_function_value) : Plugin(draw_function_value, clear_function_value, print_function_value) {
    L = luaL_newstate();
    // load libraries
    load_libraries();

    // register custom functions
    lua_register(L, "print", lua_print);
    lua_register(L, "draw", lua_draw);
    lua_register(L, "clear", lua_clear);

    // overwrite unwanted functions
    lua_register(L, "collectgarbage", lua_function_not_allowed);
    lua_register(L, "dofile", lua_function_not_allowed);
    lua_register(L, "load", lua_function_not_allowed);
    lua_register(L, "loadfile", lua_function_not_allowed);
    lua_register(L, "require", lua_function_not_allowed);
//        replace_function_in_table("os", "execute", lua_function_not_allowed); // needed in demo
    replace_function_in_table("os", "remove", lua_function_not_allowed);
    replace_function_in_table("os", "rename", lua_function_not_allowed);
    replace_function_in_table("os", "setlocale", lua_function_not_allowed);
    replace_function_in_table("os", "tmpname", lua_function_not_allowed);
    replace_function_in_table("os", "exit", lua_function_not_allowed);
    replace_function_in_table("io", "read", lua_function_not_allowed);
    replace_function_in_table("io", "write", lua_function_not_allowed);
}

LuaPlugin::~LuaPlugin() {
    lua_close(L);
}

bool LuaPlugin::load_script(const std::string &lua_script) {
    int load_stat = luaL_loadbuffer(L, lua_script.c_str(), lua_script.length(), lua_script.c_str());

    if (load_stat != LUA_OK) {
        Plugin::last_error_buffer = lua_tostring(L, -1);
    }
    return load_stat == LUA_OK;
}

bool LuaPlugin::exec_script() {
    int exec_stat = lua_pcall(L, 0, 0, 0);

    if (exec_stat != LUA_OK) {
        Plugin::last_error_buffer = lua_tostring(L, -1);
    }
    return exec_stat == LUA_OK;
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
    char *return_string;
    asprintf(&return_string, "");

    int top = lua_gettop(state);
    for (int i = 1; i <= top; i++) {  /* repeat for each level */
        int t = lua_type(state, i);

        char *string_value;
        switch (t) {
            case LUA_TSTRING:  /* strings */
                asprintf(&string_value, "%s", lua_tostring(state, i));
                break;

            case LUA_TBOOLEAN:  /* booleans */
                asprintf(&string_value, lua_toboolean(state, i) ? "true" : "false");
                break;

            case LUA_TNUMBER:  /* numbers */
                asprintf(&string_value, "%g", lua_tonumber(state, i));
                break;

            default:  /* other values */
                asprintf(&string_value, "%s", lua_typename(state, t));
                break;

        }

        char *temp;
        if (i < top) {
            asprintf(&temp, "%s%s\t", return_string, string_value);
        } else {
            asprintf(&temp, "%s%s", return_string, string_value);
        }
        free(return_string);
        free(string_value);
        return_string = temp;
    }

    if (Plugin::print_function != nullptr) {
        Plugin::print_function(return_string);
    } else {
        printf("%s", return_string);
    }
    free(return_string);
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

    Plugin::draw_function(x, y, red, green, blue, alpha, size);

    return 0;
}

int LuaPlugin::lua_clear(lua_State *state) {
    if (lua_gettop(state) != 0) {
        return luaL_error(state, "expecting no parameters");
    }

    Plugin::clear_function();

    return 0;
}

//int LuaPlugin::lua_error_handler(lua_State *L) {
//    const char *msg = lua_tostring(L, -1);
//    printf("msg: %s\n", msg);
//    luaL_traceback(L, L, msg, 2);
//    lua_print(L);
//    lua_remove(L, -2); // Remove error/"msg" from stack.
//    return 1; // Traceback is returned.
//}
