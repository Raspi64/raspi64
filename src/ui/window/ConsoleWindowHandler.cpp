//
// Created by Alex on 15.11.20.
//

#include <stdio.h>

#include "ConsoleWindowHandler.hpp"
#include "base/WindowHandler.hpp"


ConsoleWindowHandler::ConsoleWindowHandler(ConsoleWindow *window): WindowHandler(window), window(window) {}

void ConsoleWindowHandler::print(char *message) {
    // TODO
    //printf("Hello world %s", message);
}

void ConsoleWindowHandler::on_submit(void (*param)(char *)) {

}
