//
// Created by Alex on 15.11.20.
//

#include "HelpWindowHandler.hpp"
#include "base/WindowHandler.hpp"


HelpWindowHandler::HelpWindowHandler(HelpWindow *window): WindowHandler(window), window(window) {}

void HelpWindowHandler::on_request_common_root(helpentryroot_funct_t function) {
    window->on_request_common_root(function);
}

void HelpWindowHandler::on_request_language_root(helpentryroot_funct_t function) {
    window->on_request_language_root(function);
}

void HelpWindowHandler::on_search(helpentries_search_funct_t function) {
    window->on_search(function);
}
