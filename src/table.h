#ifndef TABLE_H_
#define TABLE_H_

#include "base.h"
#include "kvp.h"
#include <memory>
#include <vector>

class table
{
	private:
		static const int TABLE_SIZE = 128; //
		std::vector<std::unique_ptr<kvp>> t;
		const bitset<5> ERROR = bitset<5>(33);
		const char ERRORC = '#';

		// Private helper methods
		int hash(char c) const;
		int hash(bitset<5> b) const;
		int findPosition(char key) const;

	public:
		// Constructors and destructor
		table();
		~table() = default;  // Vector handles cleanup
		table(const table& other);
		table& operator=(const table& other);
		table(table&& other) noexcept = default;
		table& operator=(table&& other) noexcept = default;

		// Core operations
		bitset<5> get(char key, bool* found = nullptr) const;
		char get(bitset<5> value, bool* found = nullptr) const;
		bool put(char key, bitset<5> val);
		void print() const;

		// Capacity
		bool isFull() const { return t.size() >= TABLE_SIZE; }
		bool isEmpty() const { return t.empty(); }
		size_t size() const { return t.size(); }
};

#endif
