#pragma once
#include <map>
#include <vector>
#include <string>
#include "Stack.h"


class Context {
public:
	Stack* stack;
	std::map <std::string, double> *table;
	std::vector<std::string> cur_commands;

	Context();
	~Context();
};