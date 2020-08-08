#include "scanner.hpp"
#include "error_handler.hpp"

using namespace lox;

Scanner::Scanner(const std::string &source_, ErrorHandler &handler_)
    : source(source_), errorhandler(handler_)
{

    keywords["and"] = TokenType::AND;
    keywords["class"] = TokenType::CLASS;
    keywords["else"] = TokenType::ELSE;
    keywords["false"] = TokenType::FALSE;
    keywords["for"] = TokenType::FOR;
    keywords["fun"] = TokenType::FUN;
    keywords["if"] = TokenType::IF;
    keywords["nil"] = TokenType::NIL;
    keywords["or"] = TokenType::OR;
    keywords["print"] = TokenType::PRINT;
    keywords["return"] = TokenType::RETURN;
    keywords["super"] = TokenType::SUPER;
    keywords["this"] = TokenType::THIS;
    keywords["true"] = TokenType::TRUE;
    keywords["var"] = TokenType::VAR;
    keywords["while"] = TokenType::WHILE;
}

TokenList Scanner::scanTokens()
{
    while (!isAtEnd())
    {
        start = current;
        scanToken();
    }

    tokens.push_back(std::make_shared<Token>(TokenType::END_OF_FILE, "", line));
    return tokens;
}

bool Scanner::isAtEnd()
{
    return current >= source.size();
}

void Scanner::scanToken()
{
    char c = advance();

    switch (c)
    {
    case '(':
        addToken(TokenType::LEFT_PAREN);
        break;
    case ')':
        addToken(TokenType::RIGHT_PAREN);
        break;
    case '{':
        addToken(TokenType::LEFT_BRACE);
        break;
    case '}':
        addToken(TokenType::RIGHT_BRACE);
        break;
    case ',':
        addToken(TokenType::COMMA);
        break;
    case '.':
        addToken(TokenType::DOT);
        break;
    case '-':
        addToken(TokenType::MINUS);
        break;
    case '+':
        addToken(TokenType::PLUS);
        break;
    case ';':
        addToken(TokenType::SEMICOLON);
        break;
    case '*':
        addToken(TokenType::STAR);
        break;

    case '!':
        addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
        break;
    case '=':
        addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
        break;
    case '<':
        addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
        break;
    case '>':
        addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
        break;

    case '/':
        if (match('/'))
            while (peek() != '\n' && !isAtEnd())
                advance();
        else if (match('*'))
        {
            while (peek() != '*' && !isAtEnd())
            {
                advance();
                if (peek() == '*' && peekNext() == '/')
                {
                    advance();
                    advance();
                    break;
                }
            }
        }
        else
            addToken(TokenType::SLASH);
        break;
    case ' ':
    case '\r':
    case '\t':
        break;
    case '\n':
        line++;
        break;

    case '"':
        getString();
        break;
    default:
        if (isDigit(c))
            getNum();
        else if (isAlpha(c))
            identifer();
        else
        {
            std::string errorMessage = "Unexpected character: ";
            errorMessage += c;
            errorhandler.add(line, "", errorMessage);
            break;
        }
    }
}

bool Scanner::match(const char &c)
{
    if (isAtEnd())
        return false;
    if (source[current] != c)
        return false;

    current++;
    return true;
}

char Scanner::peek()
{
    if (isAtEnd())
        return '\0';
    return source[current];
}

char Scanner::peekNext()
{
    if (current + 1 >= source.size())
        return '\0';
    return source[current + 1];
}

char Scanner::advance()
{
    return source[current++];
}

void Scanner::addToken(TokenType type)
{
    addToken(type, "");
}

void Scanner::addToken(TokenType type, const std::string &literal)
{
    const std::string text = source.substr(start, current - start);
    tokens.push_back(std::make_shared<Token>(type, text, line));
}

void Scanner::getString()
{
    while (peek() != '"' && !isAtEnd())
    {
        if (peek() == '\n')
            line++;
        advance();
    }

    if (isAtEnd())
    {
        errorhandler.add(line, "", "Unterminated string.");
        return;
    }

    advance();
    const std::string literal = source.substr(start + 1, current - start - 2);
    addStrToken(literal);
}

void Scanner::addStrToken(const std::string &literal)
{
    const std::string text = source.substr(start, current - start);
    tokens.push_back(std::make_shared<StrToken>(text, literal, line));
}

bool Scanner::isDigit(const char &c)
{
    return c >= '0' && c <= '9';
}

void Scanner::getNum()
{
    while (isDigit(peek()))
        advance();

    if (peek() == '.' && isDigit(peekNext()))
        advance();
    while (isDigit(peek()))
        advance();

    const std::string literal = source.substr(start, current - start);
    addNumToken(literal);
}

void Scanner::addNumToken(const std::string &literal)
{
    const std::string text = source.substr(start, current - start);
    tokens.push_back(std::make_shared<NumToken>(text, std::stod(literal), line));
}

bool Scanner::isAlpha(const char &c)
{
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

void Scanner::identifer()
{
    while (isAlpha(peek()) || isDigit(peek()))
        advance();

    std::string text = source.substr(start, current - start);
    auto it = keywords.find(text);
    if (it != keywords.end())
        addToken(it->second);
    else
        addToken(TokenType::IDENTIFIER);
}