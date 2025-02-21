#include "tokenizer.hpp"
#include <iostream>
#include <fstream>
#include <sstream>

std::string tokensToASM(const std::vector<Token>& tokens) {     //turn the tokens into ask code
    std::string output;

    output += "global _start\n";
    output += "_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::exit) {                         //if the first term is a return
            if (tokens.size() >= 3 && (tokens[i+1].type == TokenType::int_lit) && (tokens[i+2].type == TokenType::semi)) {  //the next terms have to be an int_lit and a semi
                output += "    mov rax, 60\n";
                output += "    mov rdi, " + *tokens[i+1].value + "\n";
                output += "    syscall";
            } else {
                std::cerr << "Syntax error" << std::endl;
                exit(1);
            }
        }
    }
    return output;
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Incorrect usage: qz <input_file>" << std::endl;
        return 1;
    }

    std::ifstream input(argv[1]);
    
    if (!input) {
        std::cerr << "Error opening file: " << argv[1] << std::endl;
        return 1;
    }

    std::stringstream buffer;
    buffer << input.rdbuf();
    std::string contents = buffer.str();

    Tokenizer tokenizer(contents);
    std::vector<Token> tokens = tokenizer.tokenize();

    for (const auto& token : tokens) {                                          //printing tokens 
        std::cout << "Token: " 
                  << (token.type == TokenType::exit ? "exit" :
                      token.type == TokenType::int_lit ? "int_lit" :
                      token.type == TokenType::semi ? "semi" : "UNKNOWN")
                  << " : " << (token.value ? *token.value : "N/A") << std::endl;
    }
    
    std::string ASM = tokensToASM(tokens);

    std::cout << "\n" << ASM << std::endl;                                      //printing asm
    
    {
        std::ofstream output("output.asm");                                //create asm file

        if (!output) {                                                      // Check if file opened successfully
            std::cerr << "Error opening file" << std::endl;
            return 1;
        }

        output << ASM;                                                      // Write string to file
        
        std::cout << "Data written successfully" << std::endl;
    }

    return 0;
}