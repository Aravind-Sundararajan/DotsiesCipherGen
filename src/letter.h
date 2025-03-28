#ifndef LET_H_
#define LET_H_
#include "base.h"
#include <iostream>
#include <algorithm>

class letter {
public:
	// Data members
	std::bitset<5> b;
	
	// Constructors and destructor
	
	// Default constructor - initialize all bits to true
	letter() noexcept : b(0x1F) {} // 0x1F = 11111 in binary
	
	// Bitset constructor
	letter(const std::bitset<5>& bin) noexcept : b(bin) {}
	
	// Copy constructor
	letter(const letter& l) noexcept = default;
	
	// Move constructor
	letter(letter&& l) noexcept = default;
	
	// Copy assignment operator
	letter& operator=(const letter& l) noexcept = default;
	
	// Move assignment operator
	letter& operator=(letter&& l) noexcept = default;
	
	// Destructor
	~letter() = default;
	
	// Accessors
	
	// Get bit value using operator notation
	bool operator()(size_t x) const noexcept {
		return b[x];
	}
	
	// Get bit value
	bool get(size_t x) const noexcept {
		return b[x];
	}
	
	// Set bit value
	void set(size_t x, bool v) noexcept {
		b[x] = v;
	}
	
	// Print as boolean array (can't be constexpr due to I/O)
	void printb() const {
		for (int x = 0; x < 5; x++) {
			std::cout << (b.test(x) ? "#" : " ");
		}
		std::cout << std::endl;
	}
	
	// Bit manipulation operations
	
	// Circular shift right (clockwise)
	void circle_shift() noexcept {
		// Using bit operations for efficiency
		unsigned long val = b.to_ulong();
		// Save bit 0, shift right, place saved bit at position 4
		bool bit0 = val & 1;
		val >>= 1;
		if (bit0) val |= (1UL << 4);
		b = std::bitset<5>(val);
	}
	
	// Circular shift with direction
	void circle_shift(bool dir) noexcept {
		if (dir) {
			circle_shift(); // Right shift (clockwise)
		} else {
			// Left shift (anti-clockwise)
			unsigned long val = b.to_ulong();
			// Save bit 4, shift left, place saved bit at position 0
			bool bit4 = (val >> 4) & 1;
			val = (val << 1) & 0x1F; // Mask to keep only 5 bits
			if (bit4) val |= 1;
			b = std::bitset<5>(val);
		}
	}
	
	// Optimized circular shift by amount
	void circle_shift(bool dir, int amount) noexcept {
		// Handle negative amounts and zero
		if (amount < 0) {
			dir = !dir;
			amount = -amount;
		}
		if (amount == 0) return;
		
		// Optimize by taking modulo 5 to avoid unnecessary rotations
		amount %= 5;
		if (amount == 0) return;
		
		// Perform direct calculation instead of iterative shifts
		unsigned long val = b.to_ulong();
		if (dir) { // Right shift
			val = ((val >> amount) | (val << (5 - amount))) & 0x1F;
		} else {   // Left shift
			val = ((val << amount) | (val >> (5 - amount))) & 0x1F;
		}
		b = std::bitset<5>(val);
	}
	
	// Flip all bits
	void flip() noexcept {
		b.flip();
	}
	
	// Swap two bits
	void swap(std::size_t p1, std::size_t p2) noexcept {
		if (p1 == p2) return;
		bool temp = b[p1];
		b[p1] = b[p2];
		b[p2] = temp;
	}
	
	// Additional utility methods
	
	// Count number of set bits
	int count() const noexcept {
		return b.count();
	}
	
	// Check if any bits are set
	bool any() const noexcept {
		return b.any();
	}
	
	// Check if all bits are set
	bool all() const noexcept {
		return b.all();
	}
	
	// Check if no bits are set
	bool none() const noexcept {
		return b.none();
	}
};

#endif
