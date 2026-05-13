#include "token.hpp"
#include <iostream>
#include <cstdlib>
#include <vector>

enum class NodeKind { SET, ADD, SUB, MUL, DIV, SHOW, IF, WHILE };

struct Node {
    NodeKind kind;
    std::string varName;
    int value;
    std::vector<Node> trueBranch;
    std::vector<Node> falseBranch;
};

class Parser {
    std::vector<Token> tokens;
    int pos = 0;

    Token expect(TokenType type) {
        if (tokens[pos].type == type) return tokens[pos++];
        std::cerr << "Syntax Error on line " << tokens[pos].line 
                  << ": Expected " << static_cast<int>(type) 
                  << " but got " << static_cast<int>(tokens[pos].type) << std::endl;
        exit(1);
    }

public:
    Parser(std::vector<Token> t) : tokens(t) {}

    std::vector<Node> parse() {
        return parseBlock(TokenType::END_OF_FILE);
    }

    std::vector<Node> parseBlock(TokenType stop1, TokenType stop2 = TokenType::END_OF_FILE) {
        std::vector<Node> ast;
        while (tokens[pos].type != stop1 && tokens[pos].type != stop2 && tokens[pos].type != TokenType::END_OF_FILE) {
            
            // 1. SET command
            if (tokens[pos].type == TokenType::KW_SET) {
                expect(TokenType::KW_SET);
                std::string var = expect(TokenType::IDENTIFIER).text;
                expect(TokenType::KW_TO);
                int val = expect(TokenType::NUMBER).value;
                ast.push_back({NodeKind::SET, var, val});
            } 
            // 2. ARITHMETIC commands (ADD, SUB, MUL, DIV)
            else if (tokens[pos].type == TokenType::KW_ADD || 
                    tokens[pos].type == TokenType::KW_SUB || 
                    tokens[pos].type == TokenType::KW_MUL || 
                    tokens[pos].type == TokenType::KW_DIV) {
                
                TokenType currentType = tokens[pos].type;
                expect(currentType); // Consume the keyword
                
                int val = expect(TokenType::NUMBER).value;
                expect(TokenType::KW_TO);
                std::string var = expect(TokenType::IDENTIFIER).text;
                
                // Map the token type to the correct NodeKind
                NodeKind kind;
                if (currentType == TokenType::KW_ADD) kind = NodeKind::ADD;
                else if (currentType == TokenType::KW_SUB) kind = NodeKind::SUB;
                else if (currentType == TokenType::KW_MUL) kind = NodeKind::MUL;
                else kind = NodeKind::DIV;
                
                ast.push_back({kind, var, val});
            } 
            // 3. SHOW command
            else if (tokens[pos].type == TokenType::KW_SHOW) {
                expect(TokenType::KW_SHOW);
                std::string var = expect(TokenType::IDENTIFIER).text;
                ast.push_back({NodeKind::SHOW, var, 0});
            } 
            // 4. IF block
            else if (tokens[pos].type == TokenType::KW_IF) {
                expect(TokenType::KW_IF);
                std::string var = expect(TokenType::IDENTIFIER).text;
                expect(TokenType::KW_IS);
                int val = expect(TokenType::NUMBER).value;
                
                Node ifNode;
                ifNode.kind = NodeKind::IF;
                ifNode.varName = var;
                ifNode.value = val;
                
                // Recursive call to handle blocks inside IF
                ifNode.trueBranch = parseBlock(TokenType::KW_END, TokenType::KW_ELSE);
                
                if (tokens[pos].type == TokenType::KW_ELSE) {
                    expect(TokenType::KW_ELSE);
                    ifNode.falseBranch = parseBlock(TokenType::KW_END);
                }
                expect(TokenType::KW_END);
                ast.push_back(ifNode);
            }
            // 5. WHILE loop block
            else if (tokens[pos].type == TokenType::KW_WHILE) {
                expect(TokenType::KW_WHILE);
                std::string var = expect(TokenType::IDENTIFIER).text;
                expect(TokenType::KW_UNDER);
                int val = expect(TokenType::NUMBER).value;
                
                Node whileNode;
                whileNode.kind = NodeKind::WHILE;
                whileNode.varName = var;
                whileNode.value = val;
                
                // Recursive call to handle blocks inside WHILE
                whileNode.trueBranch = parseBlock(TokenType::KW_END);
                expect(TokenType::KW_END);
                
                ast.push_back(whileNode);
            }
            else {
                std::cerr << "Unknown command '" << tokens[pos].text << "' on line " << tokens[pos].line << std::endl;
                exit(1);
            }
        }
        return ast;
    }
};