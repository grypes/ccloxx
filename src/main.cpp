#include <fstream>
#include <iostream>
#include <sstream>
#include <streambuf>
#include <string>
#include <vector>

#include "scanner.hpp"
#include "ast.hpp"
#include "parser.hpp"
#include "interpreter.hpp"
#include "error_handler.hpp"

namespace lox
{

static void run(const std::string &source, Interpreter &interpreter)
{
    ErrorHandler errors;

    Scanner scanner(source, errors);
    std::vector<std::unique_ptr<Token>> tokens = scanner.scanTokens();

    Parser parser(std::move(tokens), errors);
    std::vector<std::shared_ptr<Stmt>> stmts = parser.parse();

    interpreter.interpret(stmts);
}

static void runFile(const std::string &path)
{
    std::ifstream file(path);
    std::ostringstream ostr;
    ostr << file.rdbuf();
    file.close();

    Interpreter interpreter;
    run(ostr.str(), interpreter);
}

static void runPrompt()
{
    Interpreter interpreter;
    while (true)
    {
        std::cout << ">";

        std::string line;
        getline(std::cin, line);
        run(line, interpreter);
    }
}
} // namespace lox

int main(int argc, const char **argv)
{
    if (argc > 2)
        std::cerr << "Usage : lox [filename]" << std::endl;
    else if (argc == 2)
    {
        lox::runFile(argv[1]);
    }
    else
    {
        lox::runPrompt();
    }
    return 0;
}