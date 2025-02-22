#pragma once

#include "tokenizer.hpp"

struct NodeExpr {
    Token int_lit;
};

struct NodeExit {
    NodeExpr expr;
};

class Parser {
public:
    inline explicit Parser(std::vector<Token> tokens) : m_tokens(std::move(tokens)) {}       //making m_tokens = tokens

    std::optional<NodeExpr> parse_expr() {                                          //parsing the expression
        if (peek().has_value() && peek().value().type == TokenType::int_lit) {      //has to be an int_lit (for now)
            return NodeExpr{consume()};
        }
        else {
            return {};
        }
    }

    std::optional<NodeExit> parse() {
        std::optional<NodeExit> exit_node;
        while (peek().has_value()) {                            //while there is a value
            if (peek().value().type == TokenType::exit) {       //if it equals exit
                consume();
                if (auto node_expr = parse_expr()) {
                    exit_node = NodeExit { .expr = node_expr.value() };
                } else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(1);
                }

                if (peek().has_value() && peek().value().type == TokenType::semi) {     //if it is a semicolon
                    consume();
                } else {
                    std::cerr << "Invalid expression" << std::endl;
                    exit(1);
                }
            } else {
                std::cerr << "Invalid expression" << std::endl;

            }
        }
        m_index = 0;
        return exit_node;
    }

private:
    [[nodiscard]] inline std::optional<Token> peek(int offset = 1) const {
        if (m_index + offset > m_tokens.size()) {
            return {};
        }
        else {
            return m_tokens.at(m_index);
        }
    }

    inline Token consume() {
        return m_tokens.at(m_index++);
    }

    const std::vector<Token> m_tokens;
    size_t m_index = 0;
};