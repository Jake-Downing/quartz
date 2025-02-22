#pragma once

#include <string>
#include <vector>
#include <optional>
#include <unordered_map>
#include <cctype>
#include <iostream>

enum class TokenType { exit, int_lit, semi };

struct Token {
    TokenType type;
    std::optional<std::string> value {};
};

class Tokenizer {
public:
    explicit Tokenizer(std::string src) : m_src(std::move(src)), m_it(m_src.begin()) {}

    std::vector<Token> tokenize() {
        std::vector<Token> tokens;
        
        while (m_it != m_src.end()) {
            char c = *m_it;

            if (std::isspace(c)) {         // Skip whitespace
                ++m_it;
                continue;
            }

            if (std::isalpha(c)) {         // Check for keywords
                std::string word = consume_word();
                if (keywords.count(word)) {
                    tokens.push_back({ keywords.at(word) });
                } else {
                    std::cerr << "Unknown keyword: " << word << std::endl;
                    exit(1);
                }
                continue;
            }

            if (std::isdigit(c)) {        // Integer literals
                tokens.push_back({ TokenType::int_lit, consume_number() });
                continue;
            }

            if (c == ';') {            // Semicolon
                tokens.push_back({ TokenType::semi });
                ++m_it;
                continue;
            }

            std::cerr << "Invalid character: " << c << std::endl;            // Unknown character
            exit(1);
        }

        return tokens;
    }

private:
    std::string m_src;
    std::string::const_iterator m_it;

    const std::unordered_map<std::string, TokenType> keywords = {
        { "exit", TokenType::exit }
    };

    std::string consume_word() {
        std::string word;
        while (m_it != m_src.end() && std::isalnum(*m_it)) {
            word.push_back(*m_it++);
        }
        return word;
    }

    std::string consume_number() {
        std::string num;
        while (m_it != m_src.end() && std::isdigit(*m_it)) {
            num.push_back(*m_it++);
        }
        return num;
    }
};
