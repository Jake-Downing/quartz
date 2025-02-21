#include <iostream>
#include <fstream>
#include <sstream>
#include <optional>
#include <vector>
#include <cstdlib>

enum class TokenType {                  //Token type 
    _return,
    int_lit,
    semi
};

struct Token {                          //
    TokenType type;
    std::optional<std::string> value;
};

bool isInt_lit(const std::string& str) {
    try {
        std::size_t pos;
        std::stoi(str, &pos);               //convert string to int
        return pos == str.size();           //check if entire string was used
    } catch (...) {
        return false;                       //stoi failed, not an integer
    }
}


void addToken(std::vector<Token>& tokens, const std::string& currentToken) {        //checks to see what token it is
    if (currentToken == "return") {
        tokens.push_back({TokenType::_return, currentToken});
    } else if (isInt_lit(currentToken)) {
        tokens.push_back({TokenType::int_lit, currentToken});
    } else if (currentToken == ";") {
        tokens.push_back({TokenType::semi, currentToken});
    } else {
        std::cerr << "Lexing error: Unrecognized token: " << currentToken << std::endl;
        exit(1);
    }
}

std::vector<Token> tokenise(const std::string& str) {            //vector that stores tokens
    std::vector<Token> tokens;
    std::string currentToken;

    for (char c : str) {
        if (isspace(c)) {                                       //tokenize on spaces
            if (!currentToken.empty()) {
                addToken(tokens, currentToken);
                currentToken.clear();                           //clear for next token
            }
        } else if (c == ';') {   // If a single-character token is found
            if (!currentToken.empty()) {
                addToken(tokens, currentToken);
                currentToken.clear();
            }
            tokens.push_back({TokenType::semi, ";"}); // Push `;` as its own token
        } else {
            currentToken += c;                                  //add character to current token
        }
    }
    
    if (!currentToken.empty()) {                                // Process the last token if there's any
        addToken(tokens, currentToken);
    }

    return tokens;
};

std::string tokensToASM(const std::vector<Token>& tokens) {     //turn the tokens into ask code
    std::string output;

    output += "global _start\n";
    output += "_start:\n";

    for (int i = 0; i < tokens.size(); i++) {
        if (tokens[i].type == TokenType::_return) {                         //if the first term is a return
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
    if (argc != 2) {                                                //check for only 1 command line arg
        std ::cerr << "Enter only 2 command line arguments. Correct usage is: qz <input.qz>" << std::endl;
        return 1;
    }

    std::string contents;
    {
        std::stringstream contentStream;
        std::fstream input(argv[1], std::ios::in);                 //(read only) command line arg file
        if (!input) {
            std::cerr << "Error opening file: " << argv[1] << std::endl;
            return 1;
        }
        contentStream << input.rdbuf();
        contents = contentStream.str();
    }

    std::vector<Token> tokens = tokenise(contents);

    std::string ASM = tokensToASM(tokens);

    for (const auto& token : tokens) {                                          //printing tokens 
        std::cout << "Token: " 
                  << (token.type == TokenType::_return ? "_return" :
                      token.type == TokenType::int_lit ? "int_lit" :
                      token.type == TokenType::semi ? "semi" : "UNKNOWN")
                  << " : " << (token.value ? *token.value : "N/A") << std::endl;
    }

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