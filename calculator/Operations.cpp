#include "Operations.h"
#include "MyException.h"
#include <iostream>
#include <cmath>

#include <vector>
#include <string>

void Plus::doing(Context & context) {
	double x1 = 0.0, x2 = 0.0;
	try {
		x1 = context.stack->pop();
		x2 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in plus");
		return;
	}
	context.stack->pushback(x1 + x2);
}

void Minus::doing(Context & context) {
	double x1 = 0.0, x2 = 0.0;
	try {
		x1 = context.stack->pop();
		x2 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in minus");
		return;
	}
	context.stack->pushback(x1 - x2);
}

void Multiply::doing(Context & context) {
	double x1 = 0.0, x2 = 0.0;
	try {
		x1 = context.stack->pop();
		x2 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in multiply");
		return;
	}
	context.stack->pushback(x1 * x2);
}


void Divide::doing(Context & context) {
	double x1 = 0.0, x2 = 0.0;
	try {
		x1 = context.stack->pop();
		x2 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in divide");
		//std::cout << e.errror() << std::endl;
		return;
	}

	if (x2 == 0.0) {
		std::cout << "DIVISION BY ZERO" << std::endl;
		return;
	}

	context.stack->pushback(x1 / x2);
}

void SQRT::doing(Context & context) {
	double x1 = 0.0;
	try {
		x1 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in sqrt");
		return;
	}
	if (x1 < 0.0) {
		throw MyException("Dividing by zero");
		return;
	}
	x1 = sqrt(x1);
	context.stack->pushback(x1);
	return;
}

void PRINT::doing(Context & context) {
	double x1 = 0.0;
	try {
		x1 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in print");
		return;
	}
	std::cout << "TOP ELEMENT: " << x1 << std::endl;
	context.stack->pushback(x1);
}

void POP::doing(Context & context) {
	double x1 = 0.0;
	try {
		x1 = context.stack->pop();
	}
	catch (MyException & e) {
		throw MyException(e.errror() + " in pop");
		return;
	}
	//std::cout << x1 << std::endl;
	return;
}

void PUSH::doing(Context & context) {
	if (context.cur_commands.size() < 2) {
		throw MyException("Not enough arguments");
		return;
	}


	std::string arg = context.cur_commands[1];
	std::map<std::string, double>::iterator it;
	it = context.table->find(arg);
	if (it == context.table->end()) {
		double num = 0.0;
		try {
			num = stod(arg, 0);
		}
		catch (std::exception & e) {
			throw MyException("No such element in define map");
			return;
		}
		context.stack->pushback(num);
		return;
	}
	else {
		context.stack->pushback((*context.table)[arg]);
	}
}

void DEFINE::doing(Context & context) {
	if (context.cur_commands.size() < 3) {
		throw MyException("Not enough arguments");
		return;
	}
	std::string arg1 = context.cur_commands[1];
	std::string arg2 = context.cur_commands[2];

	double num = 0.0;
	try {
		num = stod(arg2, 0);
	}
	catch (std::exception & e) {
		throw MyException(e.what());
		return;
	}
	(*context.table).insert( pair<string, double>(arg1, num));
	return;
}

std::string Plus::get_name() {
	return this->name;
}
std::string Minus::get_name() {
	return this->name;
}
std::string Divide::get_name() {
	return this->name;
}
std::string Multiply::get_name() {
	return this->name;
}
std::string SQRT::get_name() {
	return this->name;
}
std::string POP::get_name() {
	return this->name;
}
std::string PUSH::get_name() {
	return this->name;
}
std::string PRINT::get_name() {
	return this->name;
}
std::string DEFINE::get_name() {
	return this->name;
}

//Ctrl + d rofl