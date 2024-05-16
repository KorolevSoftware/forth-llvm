#pragma once
#include <map>
#include <string>
#include <variant>

struct Enviroment {
	std::map<std::string, std::variant<int, float, std::string>> constants;
};