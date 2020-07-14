#define _CRT_SECURE_NO_WARNINGS
#include "RNK.h"
#include <iostream>

using namespace MyBiologylib;

void print_bits(size_t);
char get_name_of_nucl(Nucl);

const int num_of_nucles_in_chain_element = sizeof(size_t) * 4;
const int num_of_bits_in_chain_element = sizeof(size_t) * 8;

//--------------PRIVATE------------------//

size_t RNK::get_nucl_mask(Nucl nucl) {
	return (size_t)nucl;
}
Nucl RNK::compl_nucl(Nucl nucl) const {
	return (Nucl)(3 - nucl);
}
void RNK::increase_chain() {
	if (this->chain == nullptr) {
		this->chain = new size_t;
		this->chain[0] = size_t(0);
		this->length = 1;
		return;
	}

	else {
		size_t* chain = new size_t[this->get_length() * 2];
		for (size_t i = 0; i < this->get_length() * 2; ++i) {
			chain[i] = size_t(0);
		}
		for (size_t i = 0; i < this->get_length(); ++i) {
			chain[i] = this->chain[i];
		}

		delete[] this->chain;
		this->chain = chain;
		this->length *= 2;
		return;
	}
	return;
}
Nucl RNK::get_nucl_name(size_t mask) const {
	return (Nucl)mask;
}

//--------------PRIVATE------------------//

//--------------PUBLIC------------------//
RNK::RNK() : chain(nullptr), num_of_nucles(0), length(0) {};
RNK::RNK(const RNK & rnk) {
	size_t* chain = new size_t[rnk.get_length()];
	for (size_t i = 0; i < rnk.get_length(); ++i) {
		chain[i] = rnk.chain[i];
	}
	this->num_of_nucles = rnk.get_num_of_nucles();
	this->length = rnk.get_length();
	this->chain = chain;
}
RNK::RNK(size_t pos, Nucl nucl) {
	this->num_of_nucles = pos + 1;
	this->length = this->get_num_of_nucles() / num_of_nucles_in_chain_element + (this->get_num_of_nucles() % num_of_nucles_in_chain_element != 0);
	size_t* chain = new size_t[this->get_length()];
	for (int i = 0; i < this->length; ++i) {
		chain[i] = size_t(0);
	}
	this->chain = chain;
	(*this)[pos] = nucl;
}
RNK::~RNK() {
	if (this->chain != nullptr) {
		delete[] this->chain;
		this->chain = nullptr;
	}
}

size_t RNK::get_length() const {
	return this->length;
}
size_t RNK::get_num_of_nucles() const {
	return this->num_of_nucles;
}

bool RNK::is_compementary(const RNK &rnk) {
	if (this->get_num_of_nucles() != rnk.get_num_of_nucles()) {
		return false;
	}
	RNK buff(rnk);
	return (!buff == (*this)) ? true : false;
}

void RNK::pushback(Nucl nucl) {
	(*this)[this->get_num_of_nucles()] = nucl;
	return;
}
void RNK::trim(size_t pos) {
	if (pos >= this->get_num_of_nucles() || pos < 0) {
		try {
			throw 1;
		}
		catch (...) {
			std::cout << "bad trim position" << std::endl;
		}

		return;
	}

	if (pos == this->get_num_of_nucles() - 1) {
		return;
	}

	size_t pos_in_chain = pos / num_of_nucles_in_chain_element;
	size_t pos_in_chain_element = pos % num_of_nucles_in_chain_element;

	size_t final_length = (pos_in_chain) + (pos_in_chain_element != 0);

	size_t* buffer = new size_t[final_length];

	for (size_t i = 0; i < final_length; ++i) {
		buffer[i] = this->chain[i];
	}

	//pulling last element of chain by zeroes

	buffer[final_length - 1] = (buffer[final_length - 1] >> (num_of_bits_in_chain_element - 2 * pos_in_chain_element)) << 
			(num_of_bits_in_chain_element - 2 * pos_in_chain_element); //put zeroes on free pos in last element of chain

	delete[] this->chain;

	this->num_of_nucles = pos;
	this->length = final_length;
	this->chain = buffer;
	return;
}								
void RNK::reverse() {
	RNK rnk(*this);
	for (size_t i = 0; i < this->get_num_of_nucles()/2; ++i) {
		Nucl tmp1, tmp2;
		tmp1 = (*this)[i];
		tmp2 = (*this)[this->get_num_of_nucles() - i - 1];
		(*this)[this->get_num_of_nucles() - i - 1] = tmp1;
		(*this)[i] = tmp2;
	}
	return;
}
splited_rnk RNK::split(size_t pos) {
	splited_rnk result;
	RNK* rnk1 = new RNK(*this);
	rnk1->trim(pos);
	RNK* rnk2 = new RNK();
	for (size_t i = pos; i < this->get_num_of_nucles(); ++i) {
		rnk2->pushback((*this)[i]);
	}
	result.rnk1 = rnk1;
	result.rnk2 = rnk2;
	return result;
}

size_t RNK::cardinality(Nucl nucl) {
	size_t result = 0;
	for (size_t i = 0; i < this->get_num_of_nucles(); ++i) {
		if ((*this)[i] == nucl) {
			++result;
		}
	}
	return result;
}
std::unordered_map< Nucl, size_t> RNK::cardinality() {
	std::unordered_map< Nucl, size_t> map;
	map[A] = cardinality(A);
	map[C] = cardinality(C);
	map[G] = cardinality(G);
	map[T] = cardinality(T);
	return map;
}

			//--------------operators------------------//

RNK::reference RNK::operator[](size_t pos) {
	return reference(pos, this);
}
Nucl RNK::operator[](size_t pos) const{
	if (pos >= this->get_num_of_nucles() || pos < 0) {
		try {
			throw 2;
		}
		catch (...) {
			std::cout << "pos = " << pos << "no such pos in chain" << std::endl;
		}
		return A;
	}

	size_t pos_in_chain = pos / num_of_nucles_in_chain_element;
	size_t pos_in_chain_element = pos % num_of_nucles_in_chain_element;
	size_t mask = 3;
	size_t result = this->chain[pos_in_chain];

	int shift = num_of_bits_in_chain_element - 2 * (pos_in_chain_element + 1);
	result >>= shift;
	result &= mask;
	return this->get_nucl_name(result);
}
RNK RNK::operator!() {
	for (size_t i = 0; i < this->get_num_of_nucles(); ++i) {
		Nucl nucl = (*this)[i];
		Nucl new_nucl = compl_nucl(nucl);
		(*this)[i] = new_nucl;
	}
	return (*this);
}
RNK RNK::operator=(const RNK & rnk) {
	if (this->get_length() != rnk.get_length()) {
		delete[] this->chain;
		this->chain = new size_t[rnk.get_length()];
		this->length = rnk.get_length();
	}

	this->num_of_nucles = rnk.get_num_of_nucles();
	for (size_t i = 0; i < this->get_length(); ++i) {
		this->chain[i] = rnk.chain[i];
	}
	return (*this);
}
RNK RNK::operator+(const RNK & rnk) {
	if (this->get_num_of_nucles() == 0) {
		RNK res(rnk);
		return res;
	}

	if (rnk.get_num_of_nucles() == 0) {
		RNK res(*this);
		return res;
	}

	RNK res(*this);
	size_t res_num_of_nucles = rnk.get_num_of_nucles() + this->get_num_of_nucles();
	size_t res_length = res_num_of_nucles / num_of_nucles_in_chain_element + (res_num_of_nucles % num_of_nucles_in_chain_element != 0);
	while (res_length  >= res.get_length()) {
		res.increase_chain();
	}

	size_t shift = res.get_num_of_nucles() % num_of_nucles_in_chain_element;
	size_t shift2 = rnk.get_num_of_nucles() % num_of_nucles_in_chain_element;
	size_t res_real_length = res.get_num_of_nucles() / num_of_nucles_in_chain_element + (res.get_num_of_nucles() % num_of_nucles_in_chain_element != 0);
	size_t rnk_real_length = rnk.get_num_of_nucles() / num_of_nucles_in_chain_element + (rnk.get_num_of_nucles() % num_of_nucles_in_chain_element != 0);

	if (shift == 0) {
		for (size_t i = 0; i < rnk_real_length; ++i) {
			res.chain[res_real_length + i] = rnk.chain[i];
		}
	}
	else {
		if (shift >= shift2) {
			for (size_t i = 0; i < rnk_real_length; ++i) {
				size_t buff = rnk.chain[i];
				buff = buff >> 2 * shift;
				res.chain[res_real_length + i - 1] += buff;

				if (i != rnk_real_length - 1) {
					buff = rnk.chain[i];
					buff = buff << (2 * (num_of_nucles_in_chain_element - shift));
					res.chain[res_real_length + i] += buff;
				}
			}
		}
		else {
			for (size_t i = 0; i < rnk_real_length; ++i) {
				size_t buff = rnk.chain[i];
				buff = buff >> 2 * shift;
				res.chain[res_real_length + i - 1] += buff;

				buff = rnk.chain[i];
				buff = buff << (2 * (num_of_nucles_in_chain_element - shift));
				res.chain[res_real_length + i] += buff;
			}
		}
	}

	res.num_of_nucles = rnk.get_num_of_nucles() + this->get_num_of_nucles();

	return res;

}
bool RNK::operator==(const RNK &rnk) const{
	if (this->get_num_of_nucles() != rnk.get_num_of_nucles()) {
		return false;
	}

	for (size_t i = 0; i < rnk.get_length(); ++i) {
		if (this->chain[i] != rnk.chain[i]) {
			return false;
		}
	}
	return true;
}
bool RNK::operator!=(const RNK &rnk) const{
	return ((*this) == rnk) ? false : true;
}
std::ostream& MyBiologylib::operator<<(std::ostream & os, const RNK & rnk) {
	for (size_t i = 0; i < rnk.get_num_of_nucles(); ++i) {
		std::cout << get_name_of_nucl(rnk[i]);
	}
	return os;
}

			//--------------operators------------------//


//--------------PUBLIC------------------//

//-----------------reference--------------------------//
RNK::reference::reference(size_t pos, RNK* rnk): position(pos), parent(rnk){
}
RNK::reference::~reference() {

}

Nucl RNK::reference::operator=(Nucl nucl) {
	this->put(nucl, this->position);
	return nucl;
}
RNK::reference::operator Nucl() const{
	if (this->position >= this->parent->get_num_of_nucles() || this->position < 0) {
		try {
			throw 3;
		}
		catch (...) {
			std::cout << "pos = " << this->position << "no such pos in chain" << std::endl;
		}
		return A;
	}

	size_t pos_in_chain = this->position / num_of_nucles_in_chain_element;
	size_t pos_in_chain_element = this->position % num_of_nucles_in_chain_element;
	size_t mask = 3;
	size_t result = this->parent->chain[pos_in_chain];

	int shift = num_of_bits_in_chain_element - 2 * (pos_in_chain_element + 1);
	result >>= shift;
	result &= mask;
	return this->parent->get_nucl_name(result);
}
void RNK::reference::put(Nucl nucl, size_t pos) const{
	while (pos / num_of_nucles_in_chain_element >= this->parent->get_length()) {
		this->parent->increase_chain();
	}

	if (this->parent->get_num_of_nucles() < pos + 1) {
		this->parent->num_of_nucles = pos + 1;
	}
	size_t mask = this->parent->get_nucl_mask(nucl);

	size_t pos_in_chain = pos / num_of_nucles_in_chain_element;					//find position in chain and in nucl
	int pos_in_chain_element = pos % num_of_nucles_in_chain_element;				//find position in chain element

	int shift = num_of_bits_in_chain_element - 2 * (pos_in_chain_element + 1);	//find shift

	size_t delete_mask = ~(0x03 << shift);										// delete mask invert smth like 00001100 and get 11110011 where 00 is pos
	mask <<= shift;

	this->parent->chain[pos_in_chain] &= delete_mask;									//deleting pos for the new element
	this->parent->chain[pos_in_chain] += mask;											//add element in the pos

	return;
}
//-----------------reference--------------------------//


//----------------------support------------------------//
void print_bits(size_t lol) {
	char* kek = new char[sizeof(size_t)*8];
	for (int i = 0; i < sizeof(size_t) * 8; ++i) {
		kek[i] = lol % 2;
		lol >>= 1;
	}
	for (int i = sizeof(size_t) * 8 - 1; i >= 0; --i) {
		std::cout << (int)kek[i];
	}
	delete[] kek;
	return;
}
char get_name_of_nucl(Nucl nucl) {
	switch (nucl) {
	case A: { return 'A'; }
	case C: { return 'C'; }
	case G: { return 'G'; }
	case T: { return 'T'; }
	default: { return 'A'; }
	}
}



//void RNK::reference::increase_chain() const {
	//	if (this->parent->chain == nullptr) {
	//		this->parent->chain = new size_t;
	//		this->parent->chain[0] = size_t(0);
	//		this->parent->length = 1;
	//		return;
	//	}
	//
	//	else {
	//		size_t* chain = new size_t[this->parent->get_length() * 2];
	//		for (size_t i = 0; i < this->parent->get_length() * 2; ++i) {
	//			chain[i] = size_t(0);
	//		}
	//		for (size_t i = 0; i < this->parent->get_length(); ++i) {
	//			chain[i] = this->parent->chain[i];
	//		}
	//
	//		delete[] this->parent->chain;
	//		this->parent->chain = chain;
	//		this->parent->length *= 2;
	//		return;
	//	}
	//	return;
	//}











//for (int i = 0; i < this->length; ++i) {
//	print_bits(this->chain[i]);
//	std::cout << " | ";
//}
//std::cout << "\n";

//for (int i = 0; i < rnk.length; ++i) {
//	print_bits(rnk.chain[i]);
//	std::cout << " | ";
//}
//std::cout << "\n";

//for (int i = 0; i < res.length; ++i) {
//	print_bits(res.chain[i]);
//	std::cout << " | ";
//}
//std::cout << "\n";
//print_chain_in_bits(*this);
//print_chain_in_bits(rnk);
//print_chain_in_bits(res);



//std::cout << "shift = " << shift << "\n";

//RNK res(*this);
//for (size_t i = 0; i < rnk.get_num_of_nucles(); ++i) {
//	res.pushback(rnk[i]);
//}
//return res;