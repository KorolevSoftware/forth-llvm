#include "token.h"

Token::Token(token_type type, std::variant<int, float, std::string> value): type(type), value(value) {}

