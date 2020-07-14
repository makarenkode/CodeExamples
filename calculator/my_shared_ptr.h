#include <iostream>

template<typename T>
class my_shared_ptr {
private:

	class Stor {
	private:
		
	public:
		size_t num_of_users;
		T* ptr;
		Stor(T* ptr) : num_of_users(1), ptr(ptr) {};
		~Stor() {
			delete this->ptr;
			this->ptr = nullptr;
		}
	};
	Stor* stor;

public:
	my_shared_ptr(T * ptr = nullptr) : stor(new Stor(ptr)) {};
	size_t use_count() {
		if (this->stor != nullptr) {
			return this->stor->num_of_users;
		}
		else {
			return 0;
		}
	}
	my_shared_ptr(const my_shared_ptr & ptr) {
		if (this->stor == nullptr) {
			this->stor = ptr.stor;
			this->stor->num_of_users += 1;
		}
		if (this->stor->ptr == ptr.stor->ptr) {
			return;
		}
		else {
			if (this->stor->num_of_users == 1) {
				delete this->stor;
			}

			this->stor = ptr.stor;
			this->stor->num_of_users += 1;
		}

	}
	my_shared_ptr operator=(my_shared_ptr & ptr) {
		if (this == ptr) {
			return *this;
		}
		if (this->stor == ptr.stor) {
			return *this;
		}
		if (this->stor->num_of_users == 1) {
			delete this->stor;
		}
		else {
			this->stor->num_of_users--;
		}
		this->stor = ptr.stor;
		ptr.stor->num_of_users++;
		return *this;
	}
	~my_shared_ptr() {
		if (this->stor->num_of_users == 1) {
			delete this->stor;
		}
		else {
			this->stor->num_of_users--;
		}
	}
	T& operator*() {
		return *(this->stor->ptr);
	}
	T* operator->() {
		return this->stor->ptr;
	}
};

