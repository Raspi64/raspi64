//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_SCHNITTSTELLE_HPP
#define IMGUI_SETUP_SCHNITTSTELLE_HPP


#include <string>
#include <plugins/Plugin.hpp>
#include "types.hpp"

class Schnittstelle {

public:

    Schnittstelle(LANG lang, void (*print_function_value)(std::string), void (*draw_function_value)(int, int, int, int, int, int, int), void (*clear_function_value)());

    enum Status {
        NOT_STARTED, // No program has been executed so far
        LOADING, // The user-program is currently loading/parsing
        LOAD_ERROR, // The user-program could not be loaded
        RUNNING, // The user-program is currently running
        RUN_ERROR, // There was an error when running the user-program
        COMPLETED_OK, // Program has exited successfully
    };

    void set_language(LANG lang);

    void start_script(const std::string &script);

    void kill_current_task();

    Status get_status();

    void save(const std::string& name, const std::string& text);

    std::string load(const std::string& name);

    Entry *get_common_help_root();

    Entry *get_language_help_root();

    std::vector<Entry *> search_entries(const std::string& searchword);

private:
    LANG current_language;
    Plugin *interpreter = nullptr;
    print_funct_t print_function;
    draw_funct_t draw_function;
    clear_funct_t clear_function;
    Schnittstelle::Status status = Schnittstelle::NOT_STARTED;
    pthread_t exec_thread{};
    bool is_running = false;

    static void *exec_script(void *params_void);

    void init_interpreter();

    Entry help_root_entry;

    void sort(std::vector<Entry> * entries);
};


#endif //IMGUI_SETUP_SCHNITTSTELLE_HPP
