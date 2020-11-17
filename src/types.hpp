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

typedef void (*keydown_function)(const SDL_Keysym keysym);


#endif
