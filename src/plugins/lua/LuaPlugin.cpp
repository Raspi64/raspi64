//
// Created by simon on 12/1/20.
//

#include <iostream>
#include "Plugin.hpp"
#include "LuaPlugin.hpp"
#include "lua_plugin.hpp"

LuaPlugin::LuaPlugin(draw_funct_t draw_function_value, clear_funct_t clear_function_value, print_funct_t print_function_value) : Plugin(draw_function_value, clear_function_value, print_function_value) {
    lua_plugin::plugin_init();
}

LuaPlugin::~LuaPlugin() {
    lua_plugin::plugin_exit();
}

bool LuaPlugin::load_script(const std::string &lua_script) {
    return lua_plugin::load_script(lua_script.c_str()) == 0;
}

bool LuaPlugin::exec_script() {
    return lua_plugin::exec_script() == 0;
}

//int LuaPlugin::lua_error_handler(lua_State *L) {
//    const char *msg = lua_tostring(L, -1);
//    printf("msg: %s\n", msg);
//    luaL_traceback(L, L, msg, 2);
//    lua_print(L);
//    lua_remove(L, -2); // Remove error/"msg" from stack.
//    return 1; // Traceback is returned.
//}
