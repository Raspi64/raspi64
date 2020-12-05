cmake_minimum_required(VERSION 3.17)
project(imgui_setup_demo)
project(imgui_setup)

set(CMAKE_CXX_STANDARD 20)

include_directories(src)
include_directories(src/gui)
include_directories(src/gui/ui)
include_directories(src/gui/ui/window)
include_directories(src/gui/ui/window/base)
include_directories(src/plugins)
include_directories(src/plugins/lua)
include_directories(src/plugins/basic)

include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(lib/imgui/misc/cpp)

include_directories(lib/ImGuiColorTextEdit)

include_directories(lib/lua-5.4/include)

include_directories(lib/my_basic)

add_executable(
        imgui_setup_demo
        demo.cpp
        # gui files
        src/gui/Gui.cpp
        src/gui/ui/UiElement.cpp
        src/gui/ui/MainMenuBar.cpp
        src/gui/ui/window/ShutdownDialogWindow.cpp
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

        # ImGuiColorTextEdit library files
        lib/ImGuiColorTextEdit/TextEditor.cpp

        # DearImGUI library files
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl2.cpp
        lib/imgui/backends/imgui_impl_sdl.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
)

add_executable(
        imgui_setup
        src/main.cpp
        # gui files
        src/gui/Gui.cpp
        src/gui/ui/UiElement.cpp
        src/gui/ui/MainMenuBar.cpp
        src/gui/ui/window/ShutdownDialogWindow.cpp
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

        # schnittstelle files
        src/Schnittstelle.cpp
        src/plugins/Plugin.cpp
        src/plugins/lua/LuaPlugin.cpp
        src/plugins/basic/BasicPlugin.cpp

        # my_basic library files
        lib/my_basic/my_basic.c

        # ImGuiColorTextEdit library files
        lib/ImGuiColorTextEdit/TextEditor.cpp

        # DearImGUI library files
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl2.cpp
        lib/imgui/backends/imgui_impl_sdl.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
        )

# Link pthread
target_link_libraries(imgui_setup "-lpthread")

# Link SDL2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
target_link_libraries(imgui_setup SDL2::Main)
target_link_libraries(imgui_setup_demo SDL2::Main)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(imgui_setup "-lGL")
    target_link_libraries(imgui_setup_demo "-lGL")
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(imgui_setup "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    target_link_libraries(imgui_setup "-L/usr/local/lib -L/opt/local/lib")
    target_link_libraries(imgui_setup_demo "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    target_link_libraries(imgui_setup_demo "-L/usr/local/lib -L/opt/local/lib")
endif()

# Link Lua 5.4
add_library(lualib SHARED IMPORTED)
if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    set_property(TARGET lualib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/lua-5.4/liblua54.so")
endif()
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    set_property(TARGET lualib PROPERTY IMPORTED_LOCATION "${CMAKE_SOURCE_DIR}/lib/lua-5.4/liblua54_darwin.a")
endif()
target_link_libraries(imgui_setup "-ldl" lualib)

add_compile_options("-Wall -Wextra -pedantic -Werror")
