#pragma once
#include <variant>
#include <map>

#include "tokenizer.h"
#include "ast_entity.h"

struct ASTBuilder {
	void build(std::vector<Token> tokens);
};