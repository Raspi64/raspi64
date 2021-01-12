//
// Created by simon on 12/1/20.
//

#include <my_basic.h>
#include <cstdarg>
#include <cstring>
#include "BasicPlugin.hpp"


BasicPlugin::BasicPlugin() : Plugin() {
    // create new state
    mb_init();
    mb_open(&bas);

    // replace stdout and stdin
    mb_set_printer(bas, BasicPlugin::basic_print);
    mb_set_inputer(bas, BasicPlugin::basic_input);

    // register custom functions
    mb_register_func(bas, "DRAW", basic_draw_pixel);
    mb_register_func(bas, "DRAW_PIXEL", basic_draw_pixel);
    mb_register_func(bas, "DRAW_RECT", basic_draw_rect);
    mb_register_func(bas, "DRAW_LINE", basic_draw_line);
    mb_register_func(bas, "DRAW_TEXT", basic_draw_text);
    mb_register_func(bas, "DRAW_CIRCLE", basic_draw_circle);
    mb_register_func(bas, "DRAW_CLEAR", basic_clear);
}

BasicPlugin::~BasicPlugin() {
    mb_close(&bas);
    mb_dispose();
}

bool BasicPlugin::load_script(const std::string &script) {
    int load_stat = mb_load_string(bas, script.c_str(), true);

    if (load_stat != MB_FUNC_OK) {
        update_error_message();
    }
    return load_stat == MB_FUNC_OK;
}

bool BasicPlugin::exec_script() {
    int exec_stat = mb_run(bas, true);

    if (exec_stat != MB_FUNC_OK) {
        update_error_message();
    }
    mb_reset(&bas, false);
    return exec_stat == MB_FUNC_OK;
}

void BasicPlugin::on_key_press(const std::string &) {
    // TODO: Implement
}

void BasicPlugin::on_key_release(const std::string &) {
    // TODO: Implement
}

std::string BasicPlugin::get_extension() {
    return ".bas";
}

std::string BasicPlugin::get_help_folder_name() {
    return "BASIC";
}

int BasicPlugin::basic_print(const char *format, ...) {
    va_list args;
    va_start(args, format);
    char *string = va_arg(args, char*);
    char *output;
    asprintf(&output, format, string);
    std::string output_string(output);
    free(output);
    Plugin::print(output_string);
    va_end(args);
    return 0;
}

int BasicPlugin::basic_input(const char *prompt, char *input, int max_length) {
    std::string tmp = Plugin::get_input_line();
    strncpy(input, tmp.c_str(), max_length);

    return tmp.length() > max_length ? max_length : (int) tmp.length();
}

void BasicPlugin::update_error_message() {
    const char *file;
    int pos;
    unsigned short row, col;
    mb_error_e error = mb_get_last_error(bas, &file, &pos, &row, &col);
    on_error(row, std::string(mb_get_error_desc(error)));
}

int BasicPlugin::basic_draw_rect(mb_interpreter_t *bas, void **ptr) {
    TGraphicRect rect{};

    int filled_int = 0;
    int thickness_int = 0;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &rect.start_x));
    mb_check(mb_pop_int(bas, ptr, &rect.start_y));
    mb_check(mb_pop_int(bas, ptr, &rect.end_x));
    mb_check(mb_pop_int(bas, ptr, &rect.end_y));
    mb_check(mb_pop_int(bas, ptr, &thickness_int));
    mb_check(mb_pop_int(bas, ptr, &filled_int));
    mb_check(mb_pop_int(bas, ptr, &rect.color.red));
    mb_check(mb_pop_int(bas, ptr, &rect.color.green));
    mb_check(mb_pop_int(bas, ptr, &rect.color.blue));
    mb_check(mb_pop_int(bas, ptr, &rect.color.alpha));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    rect.filled = filled_int != 0;
    rect.thickness = (float) thickness_int;

    Plugin::draw_rect(rect);

    return MB_FUNC_OK;
}

int BasicPlugin::basic_draw_line(mb_interpreter_t *bas, void **ptr) {
    TGraphicLine line{};

    int thickness_int = 0;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &line.start_x));
    mb_check(mb_pop_int(bas, ptr, &line.start_y));
    mb_check(mb_pop_int(bas, ptr, &line.end_x));
    mb_check(mb_pop_int(bas, ptr, &line.end_y));
    mb_check(mb_pop_int(bas, ptr, &thickness_int));
    mb_check(mb_pop_int(bas, ptr, &line.color.red));
    mb_check(mb_pop_int(bas, ptr, &line.color.green));
    mb_check(mb_pop_int(bas, ptr, &line.color.blue));
    mb_check(mb_pop_int(bas, ptr, &line.color.alpha));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    line.thickness = (float) thickness_int;

    Plugin::draw_line(line);

    return MB_FUNC_OK;
}

int BasicPlugin::basic_draw_text(mb_interpreter_t *bas, void **ptr) {
    TGraphicText text{};
    char *string;

    int size_int = 0;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &text.x));
    mb_check(mb_pop_int(bas, ptr, &text.y));
    mb_check(mb_pop_int(bas, ptr, &size_int));
    mb_check(mb_pop_string(bas, ptr, &string));
    mb_check(mb_pop_int(bas, ptr, &text.color.red));
    mb_check(mb_pop_int(bas, ptr, &text.color.green));
    mb_check(mb_pop_int(bas, ptr, &text.color.blue));
    mb_check(mb_pop_int(bas, ptr, &text.color.alpha));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    text.size = (float) size_int;
    text.text = std::string(string);

    Plugin::draw_text(text);

    return MB_FUNC_OK;
}

int BasicPlugin::basic_draw_pixel(mb_interpreter_t *bas, void **ptr) {
    TGraphicPixel pixel{};

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &pixel.x));
    mb_check(mb_pop_int(bas, ptr, &pixel.y));
    mb_check(mb_pop_int(bas, ptr, &pixel.size));
    mb_check(mb_pop_int(bas, ptr, &pixel.color.red));
    mb_check(mb_pop_int(bas, ptr, &pixel.color.green));
    mb_check(mb_pop_int(bas, ptr, &pixel.color.blue));
    mb_check(mb_pop_int(bas, ptr, &pixel.color.alpha));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    Plugin::draw_pixel(pixel);

    return MB_FUNC_OK;
}

int BasicPlugin::basic_draw_circle(mb_interpreter_t *bas, void **ptr) {
    TGraphicCircle circle{};

    int radius_int, thickness_int, filled_int;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &circle.x));
    mb_check(mb_pop_int(bas, ptr, &circle.y));
    mb_check(mb_pop_int(bas, ptr, &radius_int));
    mb_check(mb_pop_int(bas, ptr, &thickness_int));
    mb_check(mb_pop_int(bas, ptr, &filled_int));
    mb_check(mb_pop_int(bas, ptr, &circle.color.red));
    mb_check(mb_pop_int(bas, ptr, &circle.color.green));
    mb_check(mb_pop_int(bas, ptr, &circle.color.blue));
    mb_check(mb_pop_int(bas, ptr, &circle.color.alpha));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    circle.radius = (float) radius_int;
    circle.thickness = (float) thickness_int;
    circle.filled = filled_int != 0;

    Plugin::draw_circle(circle);

    return MB_FUNC_OK;
}

int BasicPlugin::basic_clear(mb_interpreter_t *bas, void **ptr) {

    mb_attempt_func_begin(bas, ptr);
    mb_attempt_func_end(bas, ptr);

    Plugin::clear();

    return MB_FUNC_OK;
}
