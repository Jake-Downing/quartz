#pragma once

#include <string>
#include <vector>
#include <optional>
#include <iostream>
#include <algorithm>

enum class TokenType { exit, int_lit, semi };

struct Token {
    TokenType type;
    std::optional<std::string> value;
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src) : m_src(std::move(src)) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        std::string buf;
    
        while (peak().has_value()) {
            char c = peak().value();
    
            
            if (std::isspace(c) || c == ';') {      //if we hit a delimiter (whitespace or ';'), process the token
                if (!buf.empty()) {
                    if (buf == "exit") {
                        tokens.push_back({TokenType::exit});
                    } else if (std::all_of(buf.begin(), buf.end(), ::isdigit)) {
                        tokens.push_back({TokenType::int_lit, buf});
                    } else {
                        std::cerr << "Lexing error: Unknown token '" << buf << "'" << std::endl;
                        exit(EXIT_FAILURE);
                    }
                    buf.clear();                //reset buffer
                }
    
                if (c == ';') {                             //if delimiter is ';', add it as a token
                    tokens.push_back({TokenType::semi});
                }
    
                consume();                             //move to next character
                continue;
            }
    
            
            buf.push_back(consume());               //add character to buffer
        }
    
        if (!buf.empty()) {                         //process last token if it exists
            if (buf == "exit") {
                tokens.push_back({TokenType::exit});
            } else if (std::all_of(buf.begin(), buf.end(), ::isdigit)) {
                tokens.push_back({TokenType::int_lit, buf});
            } else {
                std::cerr << "Lexing error: Unknown token '" << buf << "'" << std::endl;
                exit(EXIT_FAILURE);
            }
        }
    
        return tokens;
    }

private:
    std::optional<char> peak(int ahead = 1) const {
        if (m_index + ahead > m_src.length()) {
            return {};
        }
        return m_src.at(m_index);
    }

    char consume() {
        return m_src.at(m_index++);
    }

    std::string m_src;
    size_t m_index = 0;
};
