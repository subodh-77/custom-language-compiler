#include <iostream>
#include <fstream>
#include <sstream>

// Includes the source files directly to avoid duplicate compiler errors
#include "scanner.cpp"
#include "parser.cpp"
#include "asm_gen.cpp"

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: ./nlpc <input.nlp> <output.s>\n";
        return 1;
    }

    std::ifstream file(argv[1]);
    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source = buffer.str();

    Scanner scanner;
    std::vector<Token> tokens = scanner.tokenize(source);

    Parser parser(tokens);
    std::vector<Node> ast = parser.parse();

    AsmGenerator generator;
    generator.generate(ast, argv[2]);

    return 0;
}