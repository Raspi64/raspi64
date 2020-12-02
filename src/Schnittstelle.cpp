//
// Created by simon on 12/1/20.
//

#include <iostream>
#include "BasicPlugin.hpp"
#include "LuaPlugin.hpp"
#include "Plugin.hpp"
#include "Schnittstelle.hpp"


void *Schnittstelle::exec_script(void *params_void) {
    auto schnittstelle = (Schnittstelle *) params_void;

    schnittstelle->replace_status(RUNNING, "Running...");
    if (schnittstelle->interpreter->exec_script()) {
        schnittstelle->replace_status(COMPLETED_OK, "Done!");
    } else {
        schnittstelle->replace_status(RUN_ERROR, Plugin::last_error_buffer);
    }
    schnittstelle->is_running = false;

    return nullptr;
}

void Schnittstelle::start_script(const std::string &script) {
    kill_current_task();

    replace_status(LOADING, "Loading...");
    if (!interpreter->load_script(script)) {
        replace_status(LOAD_ERROR, Plugin::last_error_buffer);
        return;
    }

    // create and start thread
    is_running = true;
    pthread_create(&exec_thread, nullptr, Schnittstelle::exec_script, this);
}

void Schnittstelle::replace_status(Schnittstelle::Status new_status, const std::string &message) {
    std::cout << "Status: " << message << std::endl;
    status_message = message;
    status = new_status;
}

std::string Schnittstelle::get_status_text() {
    return status_message;
}

void Schnittstelle::set_language(LANG lang) {
    current_language = lang;
}

void Schnittstelle::init_interpreter() {
    switch (current_language) {
        case BASIC:
            interpreter = new BasicPlugin(
                    draw_function,
                    clear_function,
                    print_function
            );
            break;
        case LUA:
            interpreter = new LuaPlugin(
                    draw_function,
                    clear_function,
                    print_function
            );
            break;
        default:
            throw std::runtime_error("No interpreter!");
    }
}


Schnittstelle::Status Schnittstelle::get_status() {
    return status;
}

void Schnittstelle::kill_current_task() {
    if (is_running) {
        printf("Killing...");
        pthread_cancel(exec_thread);
        pthread_join(exec_thread, nullptr);
        is_running = false;
        printf("Killed!\n");
    }
}

Schnittstelle::Schnittstelle(
        LANG lang,
        print_funct_t print_function_value,
        draw_funct_t draw_function_value,
        clear_funct_t clear_function_value
) :
        current_language(lang),
        print_function(print_function_value),
        draw_function(draw_function_value),
        clear_function(clear_function_value) {
    init_interpreter();
}
