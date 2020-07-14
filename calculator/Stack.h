#pragma once
#include <iostream>

class Stack {
private:
	double* stack_ptr;
	size_t size;
	size_t top;
	void increase_length();

public:
	Stack();
	~Stack();

	void pushback(double);
	double pop();
	size_t get_size();
};