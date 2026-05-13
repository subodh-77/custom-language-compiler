#pragma once
#include <string>
#include <vector>

enum class TokenType {
    KW_SET, KW_ADD, KW_SUB, KW_MUL, KW_DIV, KW_SHOW, KW_TO, 
    KW_IF, KW_IS, KW_ELSE, KW_END,
    KW_WHILE, KW_UNDER,
    IDENTIFIER, NUMBER, END_OF_FILE
};

struct Token {
    TokenType type;
    std::string text;
    int value;
    int line;
};