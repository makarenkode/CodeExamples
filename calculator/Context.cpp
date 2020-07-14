#include "Context.h"
#include <string>
#include <vector>

using namespace std;

Context::Context() {
	stack = new Stack();
	table = new std::map<std::string, double>;
}
Context::~Context() {
	delete this->stack;
	delete this->table;
}