#include "tokenizer.h"
#include "map"
#include "string_view"
#include <charconv>
#include <array>
#include <functional>
#include <type_traits>
#include <regex>

namespace {
    std::map<std::string_view, token_type> forth_word = {
        {"drop", token_type::Drop },
        {"dup", token_type::Dublicate },
        {":", token_type::FunctionBegin },
        {";", token_type::FunctionEnd},
        {"swap", token_type::Swap },
        {"const", token_type::Constant },
        {"*", token_type::Multip },
      
    };

    Token word_to_keyword(std::string_view word) {
        std::map<std::string_view, token_type>::const_iterator pos = forth_word.find(word);
        if (pos == forth_word.end())
            return Token();
        
        token_type type = pos->second;
        return Token(type, std::string(word));
    }

    template<typename value_type> 
        requires std::is_integral_v<value_type> || std::is_floating_point_v<value_type>
    Token word_is_digital(std::string_view word) {
        value_type value; 
        auto [p, ec] = std::from_chars(word.data(), word.data() + word.size(), value);
        if (p == word.data() + word.size()) { // try to Float or Int
            return { token_type::Number, value };
        }

        return Token();
    }

    Token word_is_ident(std::string_view word) {
        static const std::regex r(R"(\w+)");
        if (std::regex_match(word.data(), r))
            return Token(token_type::Ident, std::string(word));
        return Token();
    }
}


Token Tokenizer::word_to_token(std::string word) {
    using word_to = std::function<Token(std::string_view)>;
    std::array<word_to, 4> word_to_functions = {
        word_is_digital<int>,
        word_is_digital<float>,
        word_to_keyword,
        word_is_ident,
    };

    Token result;
    for (const auto func : word_to_functions) {
        result = func(word);
        if (result.type != token_type::None) {
            return result;
        }
    }
    return result;
}