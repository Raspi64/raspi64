#ifndef TYPES_H
#define TYPES_H

#include <string>
#include <SDL.h>
#include <vector>


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

typedef Entry* (*helpentryroot_funct_t) ();
typedef std::vector<Entry*> (*helpentries_search_funct_t) (const std::string& searchword);

typedef void (*change_langmode_request_func_t)(LANG newLang);

void on_error(int line, const std::string &message);

void clear_error();

Entry *get_common_help_root();

Entry *get_language_help_root();

std::vector<Entry *> search_entries(const std::string &searchword);

#endif
