#ifndef ERROR_HANDLER_HPP
#define ERROR_HANDLER_HPP

#include <string>
#include <vector>

namespace lox
{
    class ErrorHandler
    {
    public:
        struct Info
        {
            size_t line;
            std::string where;
            std::string message;
        };

        ErrorHandler();
        void report();
        void add(size_t line_, const std::string &where_, const std::string &message_);

    private:
        std::vector<Info> errorList;
        bool foundError;
    };
} // namespace lox

#endif