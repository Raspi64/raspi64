//
// Created by Alex on 15.11.20.
//

#include <stdio.h>

#include "ConsoleWindowHandler.hpp"
#include "base/WindowHandler.hpp"


ConsoleWindowHandler::ConsoleWindowHandler(ConsoleWindow *window): WindowHandler(window), window(window) {}

void ConsoleWindowHandler::print(char *message) {
    //printf("Hello world %s", message);
}
