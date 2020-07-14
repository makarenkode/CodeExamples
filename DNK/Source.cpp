#define _CRT_SECURE_NO_WARNINGS
//#include "stdafx.h"
#include "gtest/gtest.h"
#include <iostream>
#include <unordered_map>
#include <stdlib.h>
#include <stdio.h>
#include "RNK.h"
#include <crtdbg.h>

using namespace std;
using namespace MyBiologylib;

TEST(test_RNK, RNK_ctor1_test) {
	RNK* rnk = new RNK();
	EXPECT_TRUE(rnk != nullptr);
	EXPECT_TRUE(rnk->get_length() == 0);
	EXPECT_TRUE(rnk->get_num_of_nucles() == 0);
}

TEST(test_RNK, RNK_ctor2_test) {
	RNK rnk;
	rnk.pushback(A);
	rnk.pushback(C);
	rnk.pushback(A);
	RNK rnk1(rnk);
	EXPECT_TRUE(rnk.get_num_of_nucles() == rnk1.get_num_of_nucles());
	EXPECT_TRUE(rnk.get_length() == rnk1.get_length());
	for (size_t i = 0; i < rnk.get_num_of_nucles(); ++i) {
		EXPECT_EQ(rnk[i], rnk1[i]);
	}
}

TEST(test_RNK, RNK_ctor3_test) {
	RNK rnk(100, C);
	EXPECT_TRUE(C == rnk[100]);
}

TEST(test_RNK, get_num_of_nucles_test) {
	RNK rnk;
	for (int i = 0; i < 1000000; ++i) {
		rnk[i] = A;
	}
	EXPECT_EQ(1000000, rnk.get_num_of_nucles());
}

TEST(test_RNK, get_length_test) {
	RNK rnk;
	for (int i = 0; i < 1000000; ++i) {
		rnk.pushback(A);
	}
	EXPECT_EQ(65536, rnk.get_length());
}

TEST(test_RNK, cardinality_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000002; ++i) { 
		rnk.pushback(Nucl(i % 4));
	}
	EXPECT_EQ(250000, rnk.cardinality(T));
	EXPECT_EQ(250001, rnk.cardinality(A));
	EXPECT_EQ(250001, rnk.cardinality(C));
	EXPECT_EQ(250000, rnk.cardinality(G));
}

TEST(test_RNK, pushback_test) {
	RNK rnk;
	for(size_t i = 0; i < 1000000; ++i)
		rnk.pushback(C);
	for (size_t i = 0; i < 1000000; ++i) {
		EXPECT_TRUE(rnk[i] == C);
	}
	
}

TEST(test_RNK, trim_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000000; ++i) {    //rnk[1] = C; rnk[5] = C; rnk[9] = C;
		rnk[i] = G;
	}
	EXPECT_TRUE(rnk.get_num_of_nucles() == 1000000);
	rnk.trim(500001);
	EXPECT_TRUE(rnk.get_num_of_nucles() == 500001);
}

TEST(test_RNK, reverse_test) {
	RNK rnk;
	for (size_t i = 0; i < 3; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK rnk1(rnk);
	rnk1.reverse();
	for (size_t i = 0; i < rnk.get_num_of_nucles(); ++i) {
		EXPECT_TRUE(rnk[i] == rnk1[rnk1.get_num_of_nucles() - 1 - i]);
	}
	EXPECT_TRUE(rnk.get_num_of_nucles() == rnk1.get_num_of_nucles());
}

TEST(test_RNK, is_сomlimentary_test) {
	RNK rnk;
	for (size_t i = 0; i < 100001; ++i) {

		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK rnk2;
	for (size_t i = 0; i < 100001; ++i) {
		switch (i % 4) {
		case 0: { rnk2.pushback(T); break; }
		case 1: { rnk2.pushback(G); break; }
		case 2: { rnk2.pushback(C); break; }
		case 3: { rnk2.pushback(A); break; }
		default: { rnk2.pushback(T); break; }
		}
	}
	EXPECT_TRUE(rnk.is_compementary(rnk2));
	rnk2[0] = A;
	EXPECT_FALSE(rnk.is_compementary(rnk2));
}

TEST(test_RNK, split_test) {
	RNK rnk;
	for (size_t i = 0; i < 500001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	size_t split_pos = 250000;
	splited_rnk rnks = rnk.split(split_pos);
	for (size_t i = 0; i < split_pos; ++i) {
		EXPECT_TRUE((*rnks.rnk1)[i] == rnk[i]);
	}

	for (size_t i = split_pos; i < rnk.get_num_of_nucles(); ++i) {
		EXPECT_TRUE((*rnks.rnk2)[i - split_pos] == rnk[i]);
	}
	EXPECT_TRUE(rnks.rnk1->get_num_of_nucles() == split_pos);
	EXPECT_TRUE(rnks.rnk2->get_num_of_nucles() == rnk.get_num_of_nucles() - split_pos);
}

TEST(test_RNK, cardinality_map_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	unordered_map< Nucl, size_t> check = rnk.cardinality();
	EXPECT_EQ(check[A], 250001);
	EXPECT_EQ(check[C], 250000);
	EXPECT_EQ(check[G], 250000);
	EXPECT_EQ(check[T], 250000);
}

TEST(test_RNK, reference_op_get_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { EXPECT_EQ(rnk[i], A); break; }
		case 1: { EXPECT_EQ(rnk[i], C); break; }
		case 2: { EXPECT_EQ(rnk[i], G); break; }
		case 3: { EXPECT_EQ(rnk[i], T); break; }
		}
	}
}
TEST(test_RNK, reference_assigment_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { EXPECT_EQ(rnk[i], A); break; }
		case 1: { EXPECT_EQ(rnk[i], C); break; }
		case 2: { EXPECT_EQ(rnk[i], G); break; }
		case 3: { EXPECT_EQ(rnk[i], T); break; }
		}
	}
}

TEST(test_RNK, op_compl_test) {

	RNK rnk;
	for (size_t i = 0; i < 100001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK compl_rnk(rnk);
	!compl_rnk;
	for (size_t i = 0; i < 100001; ++i) {
		switch (i % 4) {
		case 0: { EXPECT_EQ(compl_rnk[i], T); break; }
		case 1: { EXPECT_EQ(compl_rnk[i], G); break; }
		case 2: { EXPECT_EQ(compl_rnk[i], C); break; }
		case 3: { EXPECT_EQ(compl_rnk[i], A); break; }
		}
	}
	EXPECT_TRUE(compl_rnk.is_compementary(rnk));
}

TEST(test_RNK, op_assignment_test) {
	RNK rnk;
	for (size_t i = 0; i < 1000001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK rnk2;

	rnk2 = rnk;
	EXPECT_EQ(rnk.get_length(), rnk2.get_length());
	EXPECT_EQ(rnk.get_num_of_nucles(), rnk2.get_num_of_nucles());

	for (size_t i = 0; i < rnk2.get_num_of_nucles(); ++i) {
		EXPECT_TRUE(rnk[i] == rnk2[i]);
	}
}


TEST(test_RNK, op_plus_test) {
	RNK rnk;
	for (size_t i = 0; i < 2500018; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}

	RNK rnk1;
	for (size_t i = 0; i < 2049999; ++i) {
		switch (i % 4) {
		case 0: { rnk1.pushback(A); break; }
		case 1: { rnk1.pushback(C); break; }
		case 2: { rnk1.pushback(G); break; }
		case 3: { rnk1.pushback(T); break; }
		default: { rnk1.pushback(A); break; }
		}
	}
	RNK res;
	res = rnk + rnk1;
	EXPECT_EQ(res.get_num_of_nucles(), rnk1.get_num_of_nucles() + rnk.get_num_of_nucles());

	for (size_t i = 0; i < rnk.get_num_of_nucles(); ++i) {
		EXPECT_TRUE(res[i] == rnk[i]);
	}

	for (size_t i = rnk.get_num_of_nucles(); i < res.get_num_of_nucles(); ++i) {
		EXPECT_TRUE(res[i] == rnk1[i-rnk.get_num_of_nucles()]);
	}

	RNK rnk3;
	RNK rnk4 = rnk3 + rnk1;
	EXPECT_TRUE(rnk4 == rnk1);
	rnk4 = rnk1 + rnk3;
	EXPECT_TRUE(rnk4 == rnk1);
}

TEST(test_RNK, op_eq_test) {
	RNK rnk;
	for (size_t i = 0; i < 100001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK rnk2(rnk);
	EXPECT_TRUE(rnk2 == rnk);
	rnk2[0] = C;
	EXPECT_FALSE(rnk2 == rnk);
}

TEST(test_RNK, op_not_eq_test) {
	RNK rnk;
	for (size_t i = 0; i < 100001; ++i) {
		switch (i % 4) {
		case 0: { rnk.pushback(A); break; }
		case 1: { rnk.pushback(C); break; }
		case 2: { rnk.pushback(G); break; }
		case 3: { rnk.pushback(T); break; }
		default: { rnk.pushback(A); break; }
		}
	}
	RNK rnk2(rnk);
	EXPECT_FALSE(rnk2 != rnk);
	rnk2[0] = C;
	EXPECT_TRUE(rnk2 != rnk);
}

TEST(test_RNK, mem_leak) {
	_CrtMemState stady1 = { 0 };//Результат памяти 1 
	_CrtMemState stady2 = { 0 };//Результат памяти 2 

	RNK rnk(1, A);

	_CrtMemCheckpoint(&stady1); //Результат памяти 1



	for (size_t i = 0; i < 5000000; ++i) {
		rnk.pushback(Nucl(i % 4));
	}
	rnk.trim(1);

	
	_CrtMemCheckpoint(&stady2); //Результат памяти 2 

	_CrtMemState result = { 0 };

	EXPECT_FALSE(_CrtMemDifference(&result, &stady1, &stady2));
}


int main(int argc, char **argv) {
	::testing::InitGoogleTest(&argc, argv);
	RUN_ALL_TESTS();
	system("pause");
	return 0;
}


