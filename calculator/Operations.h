#pragma once
#include "Context.h"
#include <string>

class Operation {
public:
	virtual void doing(Context &) = 0;
	virtual std::string get_name() = 0;
};

class Plus :public Operation {
private:
	std::string name = "Plus";
public:
	std::string get_name();
	void doing(Context &);
};

class Minus :public Operation {
private:
	std::string name = "Minus";
public:
	std::string get_name();
	void doing(Context &);
};

class Multiply :public Operation {
private:
	std::string name = "Multiply";
public:
	std::string get_name();
	void doing(Context &);
};

class Divide :public Operation {
private:
	std::string name = "DIVIDE";
public:
	std::string get_name();
	void doing(Context &);
};

class SQRT :public Operation {
private:
	std::string name = "SQRT";
public:
	std::string get_name();
	void doing(Context &);
};

class PRINT : public Operation {
private:
	std::string name = "PRINT";
public:
	std::string get_name();
	void doing(Context &);
};

class POP : public Operation {
private:
	std::string name = "POP";
public:
	std::string get_name();
	void doing(Context &);
};

class PUSH : public Operation {
private:
	std::string name = "PUSH";
public:
	std::string get_name();
	void doing(Context &);
};

class DEFINE : public Operation {
private:
	std::string name = "DEFINE";
public:
	std::string get_name();
	void doing(Context &);
};
