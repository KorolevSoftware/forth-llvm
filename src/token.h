#pragma once
#include <variant>
#include <string>

enum class token_type {
	None,
	Number, // 10.0, 5, 0.6
	String, // "string"
	FunctionBegin, // :
	FunctionEnd, // ;
	Dublicate, // dup
	Drop, // drop
	Swap, // swap
	Print,
	Ident,
	Multip,
	Constant,
};

struct test {
	token_type type;
};

struct Token {
	Token() = default;
	Token(token_type type, std::variant<int, float, std::string> value);
	token_type type;
	std::variant<int, float, std::string> value;
};