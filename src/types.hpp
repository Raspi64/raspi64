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

typedef void (*keydown_function)(const SDL_Keysym keysym);
typedef void (*char_function)(std::string text);
typedef bool (*console_submit_function)(std::string text);


#endif
