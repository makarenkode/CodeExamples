#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <unordered_map>
#include <iostream>

namespace MyBiologylib {

	struct splited_rnk;
	enum Nucl { A = 0, C = 1, G = 2, T = 3 };

	class reference {};

	class RNK {
	private:
		size_t* chain;
		size_t length;
		size_t num_of_nucles;

		size_t get_nucl_mask(Nucl);
		Nucl compl_nucl(Nucl) const;
		void increase_chain();
		Nucl get_nucl_name(size_t) const;

		class reference {
		private:
			size_t position;
			RNK* parent;
			void put(Nucl, size_t) const;

		public:
			reference(size_t, RNK*);
			~reference();
			Nucl operator=(Nucl nucl);
			operator Nucl() const;
		};

	public:
		RNK();
		RNK(const RNK &);
		RNK(size_t, Nucl);
		~RNK();

		size_t get_length() const;
		size_t get_num_of_nucles() const;

		void pushback(Nucl);
		void trim(size_t);
		void reverse();
		splited_rnk split(size_t);

		bool is_compementary(const RNK &);

		size_t cardinality(Nucl);
		std::unordered_map< Nucl, size_t > cardinality();

		reference operator[](size_t);
		Nucl operator[](size_t) const;
		RNK operator!();
		RNK operator=(const RNK &);
		RNK operator+(const RNK &);
		bool operator==(const RNK &) const;
		bool operator!=(const RNK &) const;

		friend std::ostream & operator<<(std::ostream &, const RNK &);
	};

	struct splited_rnk {
		RNK *rnk1, *rnk2;
	};

};