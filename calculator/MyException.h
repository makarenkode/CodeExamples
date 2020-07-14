#pragma once
#include <exception>
#include <iostream>

using namespace std;

class MyException : public exception {
private:
	std::string mess;
public:
	MyException(const std::string & m): mess(m) {};
	std::string errror() {
		return this->mess;
	}
	~MyException() {};
};