#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <map>
#include <vector>
#include <fstream>

#include "Calc.h"
#include "gtest/gtest.h"
#include "Factories.h"
#include "Stack.h"
#include "MyException.h"
#include "my_shared_ptr.h";

using namespace std;

TEST(test_Stack, pushback_test) {
	Stack* stack = new Stack;
	for (size_t i = 0; i < 1000000; ++i) {
		stack->pushback(i + 0.1);
	}
	EXPECT_EQ(1000000, stack->get_size());
	delete stack;
}

TEST(test_Stack, pop_test) {
	Stack* stack = new Stack;
	for (size_t i = 0; i < 1000000; ++i) {
		stack->pushback(i + 0.1);
	}
	EXPECT_EQ(1000000, stack->get_size());

	for (size_t i = 0; i < 1000000; ++i) {
		EXPECT_EQ(stack->pop(), (1000000 - i - 1 + 0.1));
	}
	EXPECT_EQ(0, stack->get_size());

	delete stack;
}

TEST(test_Operation, PUSH_test) {
	Context context;

	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
	DEFINE_Factory* define_factory = new DEFINE_Factory;
	Operation* define = define_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "PUSH" };
	try {
		push->doing(context);
	}
	catch (MyException & e) {
		cout << e.errror() << endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "a" };
	try {
		push->doing(context);
	}
	catch (MyException & e) {
		cout << e.errror() << endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "1kkk" };
	try {
		push->doing(context);
	}
	catch (MyException & e) {
		cout << e.errror() << endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "DEFINE", "a", "4" };
	define->doing(context);

	context.cur_commands = { "PUSH", "a" };
	for (size_t i = 0; i < 1000000; ++i) {

		try {
			push->doing(context);
		}
		catch (MyException & e) {
			cout << e.errror() << endl;
		}
	}

	for (size_t i = 0; i < 1000000; ++i) {
		EXPECT_TRUE(4 == context.stack->pop());
	}
}

TEST(test_Operation, POP_test) {
	Context context;

	POP_Factory* pop_factory = new POP_Factory;
	Operation* pop = pop_factory->create_operation();

//___________________________________________________________//
	context.cur_commands = { "POP" };
	try {
		pop->doing(context);
	}
	catch (MyException & e) {
		cout << e.errror() << endl;
	}
//___________________________________________________________//

	for (size_t i = 0; i < 1000000; ++i) {
		context.stack->pushback(1.25);
	}
	EXPECT_TRUE(1000000 == context.stack->get_size());

	for (size_t i = 0; i < 500000; ++i) {
		try {
			pop->doing(context);
		}
		catch (MyException & e) {
			cout << e.errror() << endl;
		}
	}

	EXPECT_TRUE(500000 == context.stack->get_size());

	for (size_t i = 0; i < 500000; ++i) {
		try {
			pop->doing(context);
		}
		catch (MyException & e) {
			cout << e.errror() << endl;
		}
	}

	EXPECT_TRUE(0 == context.stack->get_size());

}

TEST(test_Operation, DEFINE_test) {
	Context context;

	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
	DEFINE_Factory* define_factory = new DEFINE_Factory;
	Operation* define = define_factory->create_operation();
//_________________________________________________________________//
	context.cur_commands = { "DEFINE" };
	try {
		define->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}

//_________________________________________________________________//
	context.cur_commands = { "DEFINE", "def" };
	try {
		define->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}

//__________________________generation_____________________________//
	size_t gen_num = 500000;
	vector<string> def_expl;
	vector<string> def_nums;
	vector<double> nums;

	char alph[] = { "abcdefghijklmnopqrstuvwxyz" };
	char numbers[] = { "01234567879" };

	//string str = "aaaaa";
	
	size_t counter = 0;
	for (int i = 0; i < 26; ++i) {
		for (int j = 0; j < 26; ++j) {
			for (int k = 0; k < 26; ++k) {
				for (int l = 0; l < 26; ++l) {
					for (int m = 0; m < 26; ++m) {
						if (counter == gen_num) {
							break;
						}
						std::string str = {};
						str += alph[i];
						str += alph[j];
						str += alph[k];
						str += alph[l];
						str += alph[m];
						
						def_expl.push_back(str);
						std::string num;
						num = std::to_string(counter);
						def_nums.push_back(num);
						nums.push_back(counter);
						if (counter % 10000 == 0) {
							cout << "def_expl = |" << str << "|; def_num = |" << num << "|; num = |" << counter << "|" << endl;

						}
						++counter;
					}
				}
			}
		}
	}
//__________________________________________________________________//

	for (size_t i = 0; i < gen_num; ++i) {
		context.cur_commands = { "DEFINE", def_expl[i], def_nums[i] };
		try {
			define->doing(context);
		}
		catch (MyException & e) {
			cout << e.errror() << endl;
		}
	}

	for (size_t i = 0; i < gen_num; ++i) {
		context.cur_commands = { "PUSH", def_expl[i] };
		try {
			push->doing(context);
		}
		catch (MyException & e) {
			cout << e.errror() << endl;
		}
	}

	for (size_t i = 0; i < gen_num; ++i) {
		EXPECT_EQ(nums[gen_num - 1 - i], context.stack->pop());
	}
}

TEST(test_Operation, PRINT_test) {
	Context context;

	PRINT_Factory* print_factory = new PRINT_Factory;
	Operation* print = print_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "PRINT" };
	try {
		print->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	
	context.cur_commands = {"PUSH", "1.25"};
	for (size_t i = 0; i < 1000; ++i) {
		push->doing(context);
	}

	context.cur_commands = { "PRINT" };
	size_t prev_size = context.stack->get_size();
	try {
		print->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
	EXPECT_EQ(prev_size, context.stack->get_size());
}

TEST(test_Operation, PLUS_test) {
	Context context;

	Plus_Factory* plus_factory = new Plus_Factory;
	Operation* plus = plus_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "+" };

	try {
		plus->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//

	context.cur_commands = { "PUSH", "8" };
	push->doing(context);

	context.cur_commands = { "+" };
	try {
		plus->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//

	context.cur_commands = { "PUSH", "8" };
	for (int i = 0; i < 1000001; ++i) {
		push->doing(context);
	}

	context.cur_commands = { "+" };
	for (int i = 0; i < 1000000; ++i) {
		plus->doing(context);
	}

	string str = "8";
	double num = stod(str, 0);

	EXPECT_EQ(num * 1000001, context.stack->pop());
}

TEST(test_Operation, MINUS_test) {
	Context context;

	Minus_Factory* minus_factory = new Minus_Factory;
	Operation* minus = minus_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "-" };
	try {
		minus->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "8" };
	push->doing(context);

	context.cur_commands = { "-" };
	try {
		minus->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "8" };
	for (int i = 0; i < 1000000; ++i) {
		push->doing(context);
	}
	context.cur_commands = { "PUSH" ,"0" };
	push->doing(context);

	context.cur_commands = { "-" };
	for (int i = 0; i < 1000000; ++i) {
		minus->doing(context);
	}

	string str = "8";
	double num = stod(str, 0);

	EXPECT_EQ(- (num * 1000000), context.stack->pop());
}

TEST(test_Operation, MULTIPLY_test) {
	Context context;
	Multiply_Factory* multi_factory = new Multiply_Factory;
	Operation* multi = multi_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "*" };
	try {
		multi->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "8" };
	push->doing(context);

	context.cur_commands = { "*" };
	try {
		multi->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "1.01" };
	for (size_t i = 0; i < 1000001; ++i) {
		push->doing(context);
	}
	context.cur_commands = { "*" };
	for (size_t i = 0; i < 1000000; ++i) {
		multi->doing(context);
	}
	//context.PRINT();

	string str = "1.01";
	double num = stod(str, 0);
	EXPECT_EQ(pow(1.01, 1000001), context.stack->pop());
}

TEST(test_Operation, DIVIDE_test) {
	Context context;
	Divide_Factory* divide_factory = new Divide_Factory;
	Operation* divide = divide_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "/" };
	try {
		divide->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "8" };
	push->doing(context);

	context.cur_commands = { "/" };
	try {
		divide->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "10" };
	for (size_t i = 0; i < 100; ++i) {
		push->doing(context);
	}
	context.cur_commands = { "PUSH", "1.e+100" };
	push->doing(context);

	context.cur_commands = { "/" };
	for (size_t i = 0; i < 100; ++i) {
		divide->doing(context);
	}
	double result = context.stack->pop();
	EXPECT_TRUE(1 < result + pow(10, -6) && 1 > result - pow(10, -6));
}

TEST(test_Operation, SQRT_test) {
	Context context;
	SQRT_Factory* sqrt_factory = new SQRT_Factory;
	Operation* sqrt = sqrt_factory->create_operation();
	PUSH_Factory* push_factory = new PUSH_Factory;
	Operation* push = push_factory->create_operation();
//_____________________________________________________________//
	context.cur_commands = { "SQRT" };
	try {
		sqrt->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "-1" };
	push->doing(context);

	context.cur_commands = { "SQRT" };
	try {
		sqrt->doing(context);
	}
	catch (MyException & e) {
		std::cout << e.errror() << std::endl;
	}
//_____________________________________________________________//
	context.cur_commands = { "PUSH", "1.e+128" };
	push->doing(context);

	context.cur_commands = { "SQRT" };
	for (int i = 0; i < 7; ++i) {
		sqrt->doing(context);
	}
	double result = context.stack->pop();
	EXPECT_TRUE(10 == result);
}



int main(int argc, char **argv) {
	//::testing::InitGoogleTest(&argc, argv);
	//RUN_ALL_TESTS();

	int* da = new int(86);
	my_shared_ptr<int>* ptr = new my_shared_ptr<int>(da);
	cout << ptr->use_count() << endl;

	std::cout << "_______________________" << endl;
	delete ptr;

	int* net = new int(798);
	my_shared_ptr<int>* ptr1 = new my_shared_ptr<int>(net);
	my_shared_ptr<int>* ptr2 = new my_shared_ptr<int>(*ptr1);
	cout << ptr1->use_count() << ' ' << ptr2->use_count() << endl;
	cout << *(*ptr1) << endl;
	delete ptr1;
	cout << ptr2->use_count() << endl;
	delete ptr2;
	cout << "______________________________________" << endl;

	double* d = new double(1.e+100);
	my_shared_ptr<double>* ptr4 = new my_shared_ptr<double>(d);
	ptr4 = ptr4;
	cout << ptr4->use_count() << endl;
	//if (false) {
	//	ifstream in("in.txt");
	//	Calc calc(in);
	//	calc.count();
	//}
	//else {
	//	ifstream in(stdin);
	//	Calc calc(in);
	//	calc.count();
	//}
//
//	
//	Calc calc(in);
//	calc.count()*/;

	system("pause");
	return 0;
}