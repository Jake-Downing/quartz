#include "tokenizer.hpp"
#include "parser.hpp"
#include "generator.hpp"

#include <iostream>
#include <fstream>
#include <sstream>


int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage: qz <input_file>" << std::endl;
        return 1;
    }


    std::string contents;
    {
        std::ifstream input(argv[1]);
        if (!input) {
            std::cerr << "Error opening file: " << argv[1] << std::endl;
            return 1;
        }

        std::stringstream buffer;
        buffer << input.rdbuf();
        contents = buffer.str();
    }

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    for (const auto& token : tokens) {                                          //printing tokens 
        std::cout << "Token: " 
                  << (token.type == TokenType::exit ? "exit" :
                      token.type == TokenType::int_lit ? "int_lit" :
                      token.type == TokenType::semi ? "semi" : "UNKNOWN")
                  << " : " << (token.value ? *token.value : "N/A") << std::endl;
    }
    
    Parser parser(std::move(tokens));
    std::optional<NodeExit> tree = parser.parse();

    if (!tree.has_value()) {
        std::cerr << "No exit statement found" << std::endl;
        exit(1);
    }

    
    Generator generator(tree.value());
    {
        std::fstream file("output.asm", std::ios::out);
        file << generator.generate();
    }

    return 0;
}