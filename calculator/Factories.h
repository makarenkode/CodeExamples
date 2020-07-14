#pragma once
#include "Stack.h"
#include "Context.h"
#include "Operations.h"
#include <map>


class Operation_Factory {
public:
	virtual Operation* create_operation() = 0;
	//virtual ~Operation_Factory() = 0;
};

class Plus_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~Plus_Factory();
};

class Minus_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~Minus_Factory();
};

class Multiply_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~Multiply_Factory();
};

class Divide_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~Divide_Factory();
};

class SQRT_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~SQRT_Factory();
};

class PRINT_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~PRINT_Factory();
};

class POP_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~POP_Factory();
};

class PUSH_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~PUSH_Factory();
};

class DEFINE_Factory : public Operation_Factory {
public:
	Operation* create_operation();
	//~DEFINE_Factory();
};

