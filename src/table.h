#ifndef TABLE_H_
#define TABLE_H_

#include "base.h"
#include "kvp.h"
#include "letter.h"
#include <memory>
#include <vector>
#include <utility>
#include <array>
#include <algorithm>
#include <functional>

// C++11 replacement for std::make_unique
namespace detail {
    template<typename T, typename... Args>
    std::unique_ptr<T> make_unique_helper(Args&&... args) {
        return std::unique_ptr<T>(new T(std::forward<Args>(args)...));
    }
}

template<typename T, typename... Args>
std::unique_ptr<T> make_unique(Args&&... args) {
    return detail::make_unique_helper<T>(std::forward<Args>(args)...);
}

class table
{
	private:
		// Table size as power of 2 for faster modulo with bit operations
		static const size_t TABLE_SIZE = 128;
		static const size_t TABLE_SIZE_MASK = TABLE_SIZE - 1;
		
		// Load factor threshold for best performance
		static const float MAX_LOAD_FACTOR;
		
		// Storage container - using vector for dynamic allocation
		std::vector<std::unique_ptr<kvp>> t;
		
		// Count of actual entries for load factor calculation
		size_t entry_count;
		
		// Private hash methods
		inline int hash(char c) const noexcept;
		inline int hash(const std::bitset<5>& b) const noexcept;
		
		// Find position with linear probing
		int findPosition(char key) const noexcept;
		
		// Find position using double hashing (more efficient)
		int findPositionDoubleHash(char key) const noexcept;

	public:
		// Constructors and destructor
		table();
		~table() = default;
		
		// Copy operations
		table(const table& other);
		table& operator=(const table& other);
		
		// Move operations
		table(table&& other) noexcept = default;
		table& operator=(table&& other) noexcept = default;

		// Core operations - all const-correct
		std::pair<bool, std::bitset<5>> get(char key) const noexcept;
		std::pair<bool, char> get(const std::bitset<5>& value) const noexcept;
		
		// Overload to get directly to a letter
		std::pair<bool, letter> get_letter(char key) const noexcept;
		
		// Put methods with different return types
		bool put(char key, const std::bitset<5>& val) noexcept;
		
		// Fast contains check
		bool contains(char key) const noexcept;
		
		// Remove an entry
		bool remove(char key) noexcept;
		
		// Clear the table
		void clear() noexcept;
		
		// Display methods
		void print() const;
		
		// Batch operations for better performance
		void put_batch(const char* keys, const std::bitset<5>* values, size_t count) noexcept;
		
		// Capacity and statistics
		bool isFull() const noexcept { return entry_count >= TABLE_SIZE * MAX_LOAD_FACTOR; }
		bool isEmpty() const noexcept { return entry_count == 0; }
		size_t size() const noexcept { return entry_count; }
		float load_factor() const noexcept { return static_cast<float>(entry_count) / TABLE_SIZE; }
		
		// Iteration helpers
		template<typename Func>
		void for_each(Func&& func) const {
			for (size_t i = 0; i < TABLE_SIZE; i++) {
				if (t[i]) {
					func(*t[i]);
				}
			}
		}
};

#endif
