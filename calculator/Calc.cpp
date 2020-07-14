#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include "Context.h"
#include "Operations.h"
#include "MyException.h"
#include "Factories.h"
#include <vector>
#include <string>
#include <map>
#include "Calc.h"
#include <fstream>

std::vector<std::string> commands = { "+", "-", "/", "*", "SQRT", "POP", "PUSH", "PRINT", "DEFINE", "EXIT" };

std::vector<std::string> Calc::parsing(std::string str) {
	std::vector<std::string> result = {};
	std::string buf;
	bool flag = false;
	for (size_t i = 0; i < str.size(); ++i) {
		if (str[i] == ' ' || i == str.size() - 1) {
			if (str[i] != ' ') {
				buf += str[i];
				result.push_back(buf);
				break;
			}
			if (flag == true) {
				result.push_back(buf);
				flag = false;
				buf = {};
			}
			//cout << flag << endl;
			continue;

		}
		flag = true;
		buf += str[i];
		//cout << flag << endl;
	}
	return result;
}

void Calc::fill_map() {
	size_t i;
	for (i = 0; i < commands.size(); ++i) {
		if (commands[i] == this->context.cur_commands[0]) {
			break;
		}
	}
	Operation* op = nullptr;
	switch (i) {
		case 0: {
			Plus_Factory *op_factory = new Plus_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 1: {
			Minus_Factory *op_factory = new Minus_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 2: {
			Divide_Factory *op_factory = new Divide_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 3: {
			Multiply_Factory *op_factory = new Multiply_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 4: {
			SQRT_Factory *op_factory = new SQRT_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 5: {
			POP_Factory *op_factory = new POP_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 6: {
			PUSH_Factory *op_factory = new PUSH_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 7: {
			PRINT_Factory *op_factory = new PRINT_Factory;
			op = op_factory->create_operation();
			break;
		}
		case 8: {
			DEFINE_Factory *op_factory = new DEFINE_Factory;
			op = op_factory->create_operation();
			break;
		}
	}
	operators.insert(std::pair<std::string, Operation*>(this->context.cur_commands[0], op));
	return;
}
Operation* Calc::get_operation() {
	std::map<std::string, Operation* >::iterator it = this->operators.find(this->context.cur_commands[0]);
	if (it == this->operators.end()) {
		this->fill_map();
	}
	return this->operators[this->context.cur_commands[0]];
}


Calc::Calc(std::ifstream & input): input(input) {
}
Calc::~Calc() {

}
	
void Calc::count() {
	while (!this->input.eof()) {
		std::string command;
		getline(this->input, command);
		this->context.cur_commands = parsing(command);
		size_t i = 0;
		bool unknown_command = false;
		for (i = 0; i < commands.size(); ++i) {
			if (this->context.cur_commands[0] == commands[i]) {
				if (i == 9) {
					return;
				}
				Operation* op = this->get_operation();
				try {
					op->doing(this->context);
				}
				catch (MyException & e) {
					std::cout << e.errror() << std::endl;
				}
				break;
			}
			if (i == 9) {
				unknown_command = true;
			}
		}
		if (unknown_command) {
			if (this->context.cur_commands[0][0] == '#') {
				//std::cout << "Comment" << std::endl;
				continue;
			}
			else {
				std::cout << "Unknown command" << std::endl;
				continue;
			}
		}
	}
	return;
}