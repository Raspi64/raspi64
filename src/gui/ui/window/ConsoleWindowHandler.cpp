//
// Created by Alex on 15.11.20.
//

#include <stdio.h>

#include "ConsoleWindowHandler.hpp"
#include "base/WindowHandler.hpp"


ConsoleWindowHandler::ConsoleWindowHandler(ConsoleWindow *window): WindowHandler(window), window(window) {}

void ConsoleWindowHandler::print(std::string message) {
    window->add_log(message);
}

void ConsoleWindowHandler::on_submit(console_submit_funct_t callback) {
    window->on_submit(callback);
}
