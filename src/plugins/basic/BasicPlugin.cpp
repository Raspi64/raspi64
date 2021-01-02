//
// Created by simon on 12/1/20.
//

#include <my_basic.h>
#include <iostream>
#include <cstdarg>
#include "BasicPlugin.hpp"


BasicPlugin::BasicPlugin() : Plugin() {
    mb_init();
    mb_open(&bas);
    mb_set_printer(bas, BasicPlugin::basic_print);

    mb_register_func(bas, "DRAW", basic_draw);
    mb_register_func(bas, "CLEAR", basic_clear);
    mb_register_func(bas, "BASICMAXIMUM", basic_maximum);
    mb_register_func(bas, "OPENTERMINAL", basic_openTerminal);
    mb_register_func(bas, "BASICECHO", basic_echo);
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
    mb_reset(&bas,false);
    return exec_stat == MB_FUNC_OK;
}

void BasicPlugin::on_key_press(const std::string &) {

}

void BasicPlugin::on_key_release(const std::string &) {

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

void BasicPlugin::update_error_message() {
    const char *file;
    int pos;
    unsigned short row, col;
    mb_error_e error = mb_get_last_error(bas, &file, &pos, &row, &col);
    on_error(row, std::string(mb_get_error_desc(error)));
}

int BasicPlugin::basic_draw(mb_interpreter_t *bas, void **ptr) {
    int result = MB_FUNC_OK;

    int x = 20;
    int y = 20;

    int red = 500;
    int green = 500;
    int blue = 500;
    int alpha = 500;
    int size = 500;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &x));
    mb_check(mb_pop_int(bas, ptr, &y));
    mb_check(mb_pop_int(bas, ptr, &red));
    mb_check(mb_pop_int(bas, ptr, &green));
    mb_check(mb_pop_int(bas, ptr, &blue));
    mb_check(mb_pop_int(bas, ptr, &alpha));
    mb_check(mb_pop_int(bas, ptr, &size));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    Plugin::draw(x, y, red, green, blue, alpha, size);

    return result;
}

int BasicPlugin::basic_clear(mb_interpreter_t *bas, void **ptr) {
    int result = MB_FUNC_OK;

   // Plugin::clear_function();

    return result;
}


int BasicPlugin::basic_maximum(mb_interpreter_t *bas, void **ptr) {
    int result = MB_FUNC_OK;
    int m = 0;
    int n = 0;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_int(bas, ptr, &m));
    mb_check(mb_pop_int(bas, ptr, &n));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    int r = n;
    if (m > n)
        r = m;

    mb_check(mb_push_int(bas, ptr, r));

    printf("param_1 %i\n", m);
    printf("param_2 %i\n", n);

    return result;
}

int BasicPlugin::basic_openTerminal(mb_interpreter_t *_bas, void **_ptr) {
    system("xfce4-terminal");
    return MB_FUNC_OK;
}

int BasicPlugin::basic_echo(mb_interpreter_t *bas, void **ptr) {
    char *string;

    mb_check(mb_attempt_open_bracket(bas, ptr));
    mb_check(mb_pop_string(bas, ptr, &string));
    mb_check(mb_attempt_close_bracket(bas, ptr));

    std::cout << string << std::endl;

    return MB_FUNC_OK;
}
