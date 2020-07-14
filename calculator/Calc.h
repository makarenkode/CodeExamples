#pragma once
#include "Context.h"
#include "Operations.h"
#include <vector>
#include <string>
#include <map>
#include <fstream>

class Calc {
private:
	std::ifstream&  input;
	Context context;
	std::map<std::string, Operation*> operators;

	std::vector<std::string> parsing(std::string str);
	//std::string get_next_string();
	Operation* get_operation();
	void fill_map();

public:
	Calc(std::ifstream & input);
	~Calc();
	void count();
};