#include "Stack.h"
#include "Context.h"
#include "Factories.h"
#include "Operations.h"

using namespace std;


Operation* Plus_Factory::create_operation() {
	return new Plus;
}

Operation* Minus_Factory::create_operation() {
	return new Minus;
}

Operation* Multiply_Factory::create_operation() {
	return new Multiply;
}

Operation* Divide_Factory::create_operation() {
	return new Divide;
}

Operation* SQRT_Factory::create_operation() {
	return new SQRT;
}


Operation* PRINT_Factory::create_operation() {
	return new PRINT;
}

Operation* POP_Factory::create_operation() {
	return new POP;
}

Operation* PUSH_Factory::create_operation() {
	return new PUSH;
}

Operation* DEFINE_Factory::create_operation() {
	return new DEFINE;
}