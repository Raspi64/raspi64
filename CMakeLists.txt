cmake_minimum_required(VERSION 3.17)
project(imgui_setup)

set(CMAKE_CXX_STANDARD 14)

include_directories(src)
include_directories(src/ui)
include_directories(src/ui/window)
include_directories(src/ui/window/base)

include_directories(lib/imgui)
include_directories(lib/imgui/backends)
include_directories(lib/imgui/misc/cpp)
include_directories(lib/imgui/misc/freetype)
include_directories(lib/ImGuiColorTextEdit)

add_executable(
        imgui_setup
        demo.cpp
        src/Gui.cpp
        src/ui/UiElement.cpp
        src/ui/MainMenuBar.cpp
        src/ui/window/ShutdownDialogWindow.cpp
        src/ui/window/HelpWindow.cpp
        src/ui/window/HelpWindowHandler.cpp
        src/ui/window/GraphicWindow.cpp
        src/ui/window/GraphicWindowHandler.cpp
        src/ui/window/EditorWindow.cpp
        src/ui/window/EditorWindowHandler.cpp
        src/ui/window/ConsoleWindow.cpp
        src/ui/window/ConsoleWindowHandler.cpp
        src/ui/window/base/Window.cpp
        src/ui/window/base/WindowHandler.cpp

        lib/ImGuiColorTextEdit/TextEditor.cpp
        lib/imgui/imgui.cpp
        lib/imgui/imgui_demo.cpp
        lib/imgui/imgui_draw.cpp
        lib/imgui/imgui_widgets.cpp
        lib/imgui/backends/imgui_impl_opengl2.cpp
        lib/imgui/backends/imgui_impl_sdl.cpp
        lib/imgui/misc/cpp/imgui_stdlib.cpp
)


# Link SDL2
list(APPEND CMAKE_MODULE_PATH ${CMAKE_CURRENT_SOURCE_DIR}/cmake)
find_package(SDL2 REQUIRED)
target_link_libraries(imgui_setup SDL2::Main)

if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
    target_link_libraries(imgui_setup "-lGL")
endif ()
if (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
    target_link_libraries(imgui_setup "-framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo")
    target_link_libraries(imgui_setup "-L/usr/local/lib -L/opt/local/lib")
endif()
