//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_SCHNITTSTELLE_HPP
#define IMGUI_SETUP_SCHNITTSTELLE_HPP


#include <string>
#include <Gui.hpp>
#include <Plugin.hpp>

class Schnittstelle {

public:

    static void init(Gui *, LANG);

    enum Status {
        NOT_STARTED, // No program has been executed so far
        LOADING, // The user-program is currently loading/parsing
        LOAD_ERROR, // The user-program could not be loaded
        RUNNING, // The user-program is currently running
        RUN_ERROR, // There was an error when running the user-program
        KILLED, // The program was stopped in mid-execution
        COMPLETED_OK, // Program has exited successfully
    };

    static void set_language(LANG lang);

    static bool start_script(const std::string &script);

    static void kill_current_task();

    static void save(const std::string &name, const std::string &text);

    static std::string load(const std::string &name);

    static bool handle_command(std::string command);

    static void on_key_press(SDL_Keysym keysym);
    static void on_key_release(SDL_Keysym keysym);

    static Entry *get_common_help_root();

    static Entry *get_language_help_root();

    static std::vector<Entry *> search_entries(const std::string &searchword);

    static void gui_draw(int x, int y, int red, int green, int blue, int alpha, int size);

    static void gui_clear();

    static void gui_print(const std::string& message);

    static void on_error(int line, const std::string &message);

    static std::string get_input_line();

    static Schnittstelle::Status status;
private:
    static Gui *gui;
    static Plugin *interpreter;
    static pthread_t exec_thread;

    static bool waiting_for_input;
    static std::string input;
    static bool input_ready;

    static void *exec_script(void *params_void);

    static Plugin* get_interpreter(LANG language);

    static Entry help_root_entry;

    static void sort_subtrees(std::vector<Entry> *entries);

    static std::string get_key_name(const SDL_Keysym &keysym);
};


#endif //IMGUI_SETUP_SCHNITTSTELLE_HPP
