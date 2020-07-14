#include "DNK.h"

DNK::DNK() {
	this->rnk1 = nullptr;
	this->rnk2 = nullptr;
}
DNK::DNK(RNK &rnk1, RNK &rnk2) {
	this->rnk1 = &rnk1;
	this->rnk2 = &rnk2;
}