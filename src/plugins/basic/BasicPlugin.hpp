//
// Created by simon on 12/1/20.
//

#ifndef IMGUI_SETUP_BASICPLUGIN_HPP
#define IMGUI_SETUP_BASICPLUGIN_HPP


#include <plugins/Plugin.hpp>
#include <my_basic.h>

class BasicPlugin : public Plugin {

public:
    BasicPlugin(
            draw_funct_t draw_function_value,
            clear_funct_t clear_function_value,
            print_funct_t print_function_value
    );

    ~BasicPlugin() override;

    bool load_script(const std::string &) override;

    bool exec_script() override;


private:
    mb_interpreter_t *bas = nullptr;

    static int basic_echo(mb_interpreter_t *bas, void **ptr);

    static int basic_openTerminal(mb_interpreter_t *bas, void **ptr);

    static int basic_maximum(mb_interpreter_t *bas, void **ptr);

    void update_error_message();

    static int basic_print(const char *format, ...);
};


#endif //IMGUI_SETUP_BASICPLUGIN_HPP
