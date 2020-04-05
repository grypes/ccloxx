#include <iostream>

#include "error_handler.hpp"

using namespace lox;

ErrorHandler::ErrorHandler() : errorList(), foundError(false) {}

void ErrorHandler::report()
{
    for (auto error : errorList)
    {
        std::cout << "[line " + std::to_string(error.line) + "] Error" + error.where + ": " + error.message << std::endl;
    }
}

void ErrorHandler::add(int line_, const std::string &where_, const std::string &message_)
{
    errorList.push_back({line_, where_, message_});
    foundError = true;
}
