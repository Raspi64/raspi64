#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <SDL.h>
#include <vector>


struct TColor {
    int red;
    int green;
    int blue;
    int alpha;
};

struct TGraphicEntity {
    TColor color;
};

struct TGraphicPixel {
    int x;
    int y;
    int size;
    TColor color;
};

struct TGraphicCircle: TGraphicEntity {
    int x;
    int y;
    float radius;
    bool filled;
    float thickness;
};

struct TGraphicRect: TGraphicEntity {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    bool filled;
    float thickness;
};

struct TGraphicLine: TGraphicEntity {
    int start_x;
    int start_y;
    int end_x;
    int end_y;
    float thickness;
};

struct TGraphicText: TGraphicEntity {
    int x;
    int y;
    float size;
    char* text;
};



enum TGraphicEntityType {
    TYPE_RECT,
    TYPE_CIRCLE,
    TYPE_LINE,
    TYPE_TEXT,
};

struct TGraphicDrawEntity {
    TGraphicEntity* entity;
    TGraphicEntityType type;
};


enum LANG {
    BASIC,
    LUA,
};

struct Entry {
    bool is_file;
    std::string name;
    std::vector<std::string> searchWords;
    std::string content;
    std::vector<Entry> sub_entries;
};

typedef void (*char_funct_t)(std::string text);

typedef void (*keydown_funct_t)(const SDL_Keysym keysym);

typedef bool (*console_submit_funct_t)(std::string text);

typedef void (*print_funct_t)(std::string);

typedef void (*clear_funct_t)();

typedef void (*draw_funct_t)(int x, int y, int red, int green, int blue, int alpha, int size);

typedef Entry *(*helpentryroot_funct_t)();

typedef std::vector<Entry *> (*helpentries_search_funct_t)(const std::string &searchword);

typedef void (*change_langmode_request_func_t)(LANG newLang);

#endif
