#include <iostream>
#include "MyException.h"
#include "Stack.h"

void Stack::increase_length() {
	if (this->stack_ptr == nullptr) {
		this->stack_ptr = new double[1];
		this->size = 1;
	}
	else {
		double* ptr = new double[this->size * 2];
		for (size_t i = 0; i < this->size * 2; ++i) {
			ptr[i] = double(0);
		}
		for (size_t i = 0; i < this->size; ++i) {
			ptr[i] = this->stack_ptr[i];
		}
		delete[] this->stack_ptr;
		this->stack_ptr = ptr;
		this->size *= 2;
	}
	return;
}

Stack::Stack() : stack_ptr(nullptr), size(0), top(-1) {};
Stack::~Stack() {
	delete[] stack_ptr;
}

void Stack::pushback(double elem) {
	if (this->top + 1 == this->size) {
		this->increase_length();
	}
	++this->top;
	this->stack_ptr[this->top] = elem;
	return;
}

double Stack::pop() {
	if (this->top == -1) {
		throw MyException("Stack is empty");
		//return 0.0;
	}
	double result = this->stack_ptr[this->top];
	--this->top;
	return result;
}

size_t Stack::get_size() {
	return this->top + 1;
}
