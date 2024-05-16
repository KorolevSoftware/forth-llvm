#include <fstream>
#include <vector>
#include <string>
#include <iterator>
#include <ranges>
#include <sstream>
#include <iostream>
#include <string_view>
#include "tokenizer.h"
#include "ast_builder.h"

const auto to_tokens = std::views::transform([](std::string in) {
    return Tokenizer::word_to_token(in);
});

extern "C" {
    void dup();
}

int main(int argn, char* args[]) {
    dup();
    //push_integer(10);
    ASTBuilder buildr;
    std::ifstream is("../tests/call_function2.fr");
    auto result = std::views::istream<std::string>(is) | to_tokens;
    
    std::vector<Token> tokens;

    for (auto token : result) {
        tokens.push_back(token);
    }
    buildr.build(tokens);

    for (auto n : result) {
        std::cout << n.value.index() << " ";
    }
    return 0;
}