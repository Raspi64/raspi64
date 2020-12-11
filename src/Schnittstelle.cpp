//
// Created by simon on 12/1/20.
//

#include <iostream>
#include <fstream>
#include "BasicPlugin.hpp"
#include "LuaPlugin.hpp"
#include "Plugin.hpp"
#include "Schnittstelle.hpp"


void *Schnittstelle::exec_script(void *params_void) {
    auto schnittstelle = (Schnittstelle *) params_void;

    schnittstelle->status = RUNNING;
    if (schnittstelle->interpreter->exec_script()) {
        schnittstelle->status = COMPLETED_OK;
    } else {
        schnittstelle->status = RUN_ERROR;
    }
    schnittstelle->is_running = false;

    return nullptr;
}

void Schnittstelle::start_script(const std::string &script) {
    kill_current_task();

    status = LOADING;
    if (!interpreter->load_script(script)) {
        status = LOAD_ERROR;
        return;
    }

    // create and start thread
    is_running = true;
    pthread_create(&exec_thread, nullptr, Schnittstelle::exec_script, this);
}

void Schnittstelle::set_language(LANG lang) {
    current_language = lang;
    init_interpreter();
}

void Schnittstelle::init_interpreter() {
    kill_current_task();
    delete interpreter;

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

void Schnittstelle::save(const std::string &name, const std::string &text) {
    std::ofstream outfile;
    outfile.open("../saves/" + name);
    outfile << text;
    outfile.close();
}

std::string Schnittstelle::load(const std::string &name) {
    std::string text;
    std::ifstream infile;
    infile.open("../saves/" + name);
    infile >> text;
    infile.close();
    return text;
}
