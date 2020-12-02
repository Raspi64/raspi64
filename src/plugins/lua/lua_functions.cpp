
#include <cstdio>
#include <cstdlib>
#include <lua.hpp>
#include <plugins/Plugin.hpp>
#include "lua_functions.hpp"

namespace lua_plugin {
    int lua_function_not_allowed(lua_State *state) {
        return luaL_error(state, "this function is not allowed");
    }

    int lua_print(lua_State *state) {
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

    int lua_draw(lua_State *state) {
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

    int lua_clear(lua_State *state) {
        if (lua_gettop(state) != 0) {
            return luaL_error(state, "expecting no parameters");
        }

        Plugin::clear_function();

        return 0;
    }
};