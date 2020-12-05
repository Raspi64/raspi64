#ifndef TYPES_H
#define TYPES_H

#include <SDL.h>


struct TGraphicPixel {
    int size;
    int x;
    int y;
    int red;
    int green;
    int blue;
    int alpha;
};

enum LANG {
    BASIC,
    LUA,
};

typedef void (*keydown_funct_t)(const SDL_Keysym keysym);
typedef void (*char_funct_t)(std::string text);
typedef bool (*console_submit_funct_t)(std::string text);

typedef void (*print_funct_t)(std::string);

typedef void (*draw_funct_t)(int x, int y, int red, int green, int blue, int alpha, int size);

typedef void (*clear_funct_t)();


#endif
