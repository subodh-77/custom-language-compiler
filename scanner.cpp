#include "token.hpp"
#include <cctype>
#include <iostream>

class Scanner {
public:
    std::vector<Token> tokenize(const std::string& source) {
        std::vector<Token> tokens;

        int i = 0;
        int line = 1;

        while (i < source.length()) {

            char c = source[i];
            if (c == '\n') {
                line++;
                i++;
                continue;
            }
            if (isspace(c)) {
                i++;
                continue;
            }

            // (letter | _) (letter | digit | _)*
            if (isalpha(c) || c == '_') {

                std::string word;

                while (i < source.length() &&
                      (isalnum(source[i]) || source[i] == '_')) {

                    word += source[i];
                    i++;
                }

                // Keywords
                if (word == "set")
                    tokens.push_back({TokenType::KW_SET, word, 0, line});

                else if (word == "add")
                    tokens.push_back({TokenType::KW_ADD, word, 0, line});

                else if (word == "subtract")
                    tokens.push_back({TokenType::KW_SUB, word, 0, line});

                else if (word == "multiply")
                    tokens.push_back({TokenType::KW_MUL, word, 0, line});

                else if (word == "divide")
                    tokens.push_back({TokenType::KW_DIV, word, 0, line});

                else if (word == "show" || word == "print")
                    tokens.push_back({TokenType::KW_SHOW, word, 0, line});

                else if (word == "to")
                    tokens.push_back({TokenType::KW_TO, word, 0, line});

                else if (word == "if")
                    tokens.push_back({TokenType::KW_IF, word, 0, line});

                else if (word == "is")
                    tokens.push_back({TokenType::KW_IS, word, 0, line});

                else if (word == "else")
                    tokens.push_back({TokenType::KW_ELSE, word, 0, line});

                else if (word == "end")
                    tokens.push_back({TokenType::KW_END, word, 0, line});

                else if (word == "while")
                    tokens.push_back({TokenType::KW_WHILE, word, 0, line});

                else if (word == "under")
                    tokens.push_back({TokenType::KW_UNDER, word, 0, line});

                // Identifier
                else
                    tokens.push_back({
                        TokenType::IDENTIFIER,
                        word,
                        0,
                        line
                    });

                continue;
            }

            // NUMBER
            if (isdigit(c)) {

                std::string num;

                while (i < source.length() &&
                       isdigit(source[i])) {

                    num += source[i];
                    i++;
                }

                tokens.push_back({
                    TokenType::NUMBER,
                    num,
                    std::stoi(num),
                    line
                });

                continue;
            }

            // Ignore unknown characters
            i++;
        }

        // EOF Token
        tokens.push_back({
            TokenType::END_OF_FILE,
            "",
            0,
            line
        });

        return tokens;
    }
};