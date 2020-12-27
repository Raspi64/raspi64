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

//    mb_register_func(bas, "DRAW", basic_draw); // TODO
//    mb_register_func(bas, "CLEAR", basic_clear); // TODO

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
    return exec_stat == MB_FUNC_OK;
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
