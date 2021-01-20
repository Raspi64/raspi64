cmake_minimum_required(VERSION 3.13)
project(raspi64)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/../)

set(CMAKE_CXX_STANDARD 17)

include_directories(src)
include_directories(src/gui)
include_directories(src/gui/extension)
include_directories(src/gui/ui)
include_directories(src/gui/ui/window/base)
include_directories(src/gui/ui/window)
include_directories(src/gui/ui/window/dialog)
include_directories(src/plugins)
include_directories(src/plugins/lua)
include_directories(src/plugins/basic)

include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(lib/imgui/misc/cpp)

include_directories(lib/imgui_color_text_editor)

include_directories(lib/lua-5.4)

include_directories(lib/my_basic)

add_executable(
        raspi64
        src/main.cpp
        # gui files
        src/gui/Gui.cpp
        src/gui/ui/UiElement.cpp
        src/gui/ui/MainMenuBar.cpp
        src/gui/ui/window/dialog/ShutdownDialogWindow.cpp
        src/gui/ui/window/dialog/ChangeLangModeDialogWindow.cpp
        src/gui/ui/window/HelpWindow.cpp
        src/gui/ui/window/HelpWindowHandler.cpp
        src/gui/ui/window/GraphicWindow.cpp
        src/gui/ui/window/GraphicWindowHandler.cpp
        src/gui/ui/window/EditorWindow.cpp
        src/gui/ui/window/EditorWindowHandler.cpp
        src/gui/ui/window/ConsoleWindow.cpp
        src/gui/ui/window/ConsoleWindowHandler.cpp
        src/gui/ui/window/base/Window.cpp
        src/gui/ui/window/base/WindowHandler.cpp

        src/gui/extension/TextEditorLanguageExtension.cpp

        # schnittstelle files
        src/Kommunikationsstelle.cpp
        src/plugins/Plugin.cpp
        src/plugins/lua/LuaPlugin.cpp
        src/plugins/basic/BasicPlugin.cpp

        # lua library files
        lib/lua-5.4/lapi.c
        lib/lua-5.4/lcode.c
        lib/lua-5.4/lctype.c
        lib/lua-5.4/ldebug.c
        lib/lua-5.4/ldo.c
        lib/lua-5.4/ldump.c
        lib/lua-5.4/lfunc.c
        lib/lua-5.4/lgc.c
        lib/lua-5.4/llex.c
        lib/lua-5.4/lmem.c
        lib/lua-5.4/lobject.c
        lib/lua-5.4/lopcodes.c
        lib/lua-5.4/lparser.c
        lib/lua-5.4/lstate.c
        lib/lua-5.4/lstring.c
        lib/lua-5.4/ltable.c
        lib/lua-5.4/ltm.c
        lib/lua-5.4/lundump.c
        lib/lua-5.4/lvm.c
        lib/lua-5.4/lzio.c
        lib/lua-5.4/lauxlib.c
        lib/lua-5.4/lbaselib.c
        lib/lua-5.4/lcorolib.c
#        lib/lua-5.4/ldblib.c
#        lib/lua-5.4/liolib.c
        lib/lua-5.4/lmathlib.c
        lib/lua-5.4/loadlib.c
        lib/lua-5.4/loslib.c
        lib/lua-5.4/lstrlib.c
        lib/lua-5.4/ltablib.c
#        lib/lua-5.4/lutf8lib.c
        lib/lua-5.4/linit.c

        # help system files
        src/HelpSystem.cpp

        # my_basic library files
        lib/my_basic/my_basic.c

        # imgui_color_text_editor library files
        lib/imgui_color_text_editor/TextEditor.cpp

        # DearImGUI library files
        lib/imgui/imgui.cpp
#        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl2.cpp
        lib/imgui/backends/imgui_impl_sdl.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
)

# Link pthread
target_link_libraries(raspi64 "-lpthread")

# Link SDL2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
target_link_libraries(raspi64 SDL2::Main)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(raspi64 "-lGL")
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(raspi64 "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    target_link_libraries(raspi64 "-L/usr/local/lib -L/opt/local/lib")
endif ()

# Link Filesystem (gcc 8 an below)
# not needed on macos
if (NOT ${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(raspi64 -lstdc++fs)
endif ()

add_compile_options("-Wall -Wextra -pedantic -Werror")
