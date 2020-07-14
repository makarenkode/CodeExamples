#pragma once
#include "RNK.h"

using namespace MyBiologylib;

class DNK {
private:
	RNK *rnk1, *rnk2;
public:
	DNK();
	DNK(RNK &, RNK &);
};