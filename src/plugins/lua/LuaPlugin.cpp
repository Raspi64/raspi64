//
// Created by simon on 12/1/20.
//

#include <lua.hpp>
#include <iostream>
#include "Plugin.hpp"
#include "LuaPlugin.hpp"

LuaPlugin::LuaPlugin(draw_funct_t draw_function_value, clear_funct_t clear_function_value, print_funct_t print_function_value) : Plugin(draw_function_value, clear_function_value, print_function_value) {
    L = luaL_newstate();
    luaopen_base(L);
    // luaopen_package(L); // blocked
    luaopen_coroutine(L);
    luaopen_table(L);
    // luaopen_io(L); // blocked
    luaopen_os(L);
    luaopen_string(L);
    luaopen_math(L);
    luaopen_utf8(L);
    // luaopen_debug(L); // blocked

    lua_register(L, "print", LuaPlugin::lua_print);
//    replace_function_in_table("os", "exit", LuaPlugin::lua_function_not_allowed);
//    replace_function_in_table("io", "read", LuaPlugin::lua_function_not_allowed);
//    replace_function_in_table("io", "write", LuaPlugin::lua_function_not_allowed);
    lua_register(L, "draw", LuaPlugin::lua_function_not_allowed);
    lua_register(L, "clear", LuaPlugin::lua_clear);
}

LuaPlugin::~LuaPlugin() {
    lua_close(L);
}

bool LuaPlugin::load_script(const std::string &lua_script) {
    int load_stat = luaL_loadbuffer(L, lua_script.c_str(), lua_script.length(), lua_script.c_str());

    if (load_stat != LUA_OK) {
        set_error_message(lua_tostring(L, -1));
    }
    return load_stat == LUA_OK;
}

bool LuaPlugin::exec_script() {
    return lua_pcall(L, 0, 0, 0) == LUA_OK;
}

void LuaPlugin::set_error_message(const std::string &error_message) {
    last_error_buffer = error_message;

    // TODO: Implement line parsing
    last_error_line = 0;
}

void LuaPlugin::replace_function_in_table(const std::string &table, const std::string &field, lua_CFunction function) {
    lua_getglobal(L, table.c_str());
    lua_pushstring(L, field.c_str());
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

        switch (t) {
            case LUA_TSTRING:  /* strings */
                return_string += lua_tostring(state, i);
                break;
            case LUA_TBOOLEAN:  /* booleans */
                return_string += lua_toboolean(state, i) ? "true" : "false";
                break;
            case LUA_TNUMBER:  /* numbers */
                return_string += lua_tonumber(state, i);
                break;
            default:  /* other values */
                return_string += lua_typename(state, t);
                break;
        }
        if (i < top) {
            return_string += '\t';
        }
    }

    Plugin::print_function(return_string);
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

int LuaPlugin::lua_error_handler(lua_State *L) {
    luaL_where(L, 0);
    const char *pos = lua_tostring(L, -1);
    printf("pos: %s\n", pos);
    const char *msg = lua_tostring(L, -1);
    printf("msg: %s\n", msg);
    luaL_traceback(L, L, msg, 2);
    printf("Stack: ");
    lua_print(L);
    lua_remove(L, -2); // Remove error/"msg" from stack.
    return 1; // Traceback is returned.
}
