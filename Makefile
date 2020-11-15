
#CXX = g++
#CXX = clang++

EXE = imgui_new_demo
IMGUI_DIR = ./lib/imgui
SRC_DIR = ./src
UI_DIR = $(SRC_DIR)/ui
WINDOW_DIR = $(UI_DIR)/window
SOURCES = demo.cpp
SOURCES += src/Gui.cpp $(UI_DIR)/UiElement.cpp $(WINDOW_DIR)/base/Window.cpp $(WINDOW_DIR)/base/WindowHandler.cpp
SOURCES += $(UI_DIR)/MainMenuBar.cpp
SOURCES += $(WINDOW_DIR)/EditorWindow.cpp
SOURCES += $(WINDOW_DIR)/EditorWindowHandler.cpp
SOURCES += $(WINDOW_DIR)/HelpWindow.cpp
SOURCES += $(WINDOW_DIR)/HelpWindowHandler.cpp
SOURCES += $(WINDOW_DIR)/GraphicWindow.cpp
SOURCES += $(WINDOW_DIR)/GraphicWindowHandler.cpp
SOURCES += $(WINDOW_DIR)/ConsoleWindow.cpp
SOURCES += $(WINDOW_DIR)/ConsoleWindowHandler.cpp
SOURCES += $(WINDOW_DIR)/ShutdownDialogWindow.cpp
SOURCES += $(IMGUI_DIR)/imgui.cpp $(IMGUI_DIR)/imgui_demo.cpp $(IMGUI_DIR)/imgui_draw.cpp $(IMGUI_DIR)/imgui_widgets.cpp
SOURCES += $(IMGUI_DIR)/backends/imgui_impl_sdl.cpp $(IMGUI_DIR)/backends/imgui_impl_opengl2.cpp
OBJS = $(addsuffix .o, $(basename $(notdir $(SOURCES))))
UNAME_S := $(shell uname -s)

CXXFLAGS = -I$(IMGUI_DIR) -I$(IMGUI_DIR)/backends
CXXFLAGS += -g -Wall -Wformat
LIBS =

##---------------------------------------------------------------------
## BUILD FLAGS PER PLATFORM
##---------------------------------------------------------------------

ifeq ($(UNAME_S), Linux) #LINUX
	ECHO_MESSAGE = "Linux"
	LIBS += -lGL -ldl `sdl2-config --libs`

	CXXFLAGS += `sdl2-config --cflags`
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(UNAME_S), Darwin) #APPLE
	ECHO_MESSAGE = "Mac OS X"
	LIBS += -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo `sdl2-config --libs`
	LIBS += -L/usr/local/lib -L/opt/local/lib

	CXXFLAGS += `sdl2-config --cflags`
	CXXFLAGS += -I/usr/local/include -I/opt/local/include
	CFLAGS = $(CXXFLAGS)
endif

ifeq ($(findstring MINGW,$(UNAME_S)),MINGW)
	ECHO_MESSAGE = "MinGW"
	LIBS += -lgdi32 -lopengl32 -limm32 `pkg-config --static --libs sdl2`

	CXXFLAGS += `pkg-config --cflags sdl2`
	CFLAGS = $(CXXFLAGS)
endif

##---------------------------------------------------------------------
## BUILD RULES
##---------------------------------------------------------------------

%.o:%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:src/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:src/ui/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:src/ui/window/base/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:src/ui/window/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

%.o:$(IMGUI_DIR)/backends/%.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

all: $(EXE)
	@echo Build complete for $(ECHO_MESSAGE)

$(EXE): $(OBJS)
	$(CXX) -o $@ $^ $(CXXFLAGS) $(LIBS)

clean:
	rm -f $(EXE) $(OBJS)
