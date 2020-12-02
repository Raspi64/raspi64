#ifndef LUATEST_LUA_PLUGIN_H
#define LUATEST_LUA_PLUGIN_H

#include "Plugin.hpp"

namespace lua_plugin {

    void plugin_init();

    void plugin_exit();

    int load_script(const char *lua_script);

    int exec_script();

}

#endif //LUATEST_LUA_PLUGIN_H
