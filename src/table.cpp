#include "table.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>
#include <cmath>

// Definition of static constant
const float table::MAX_LOAD_FACTOR = 0.75f;

// Constructor - Initialize empty table with entry count at 0
table::table() : t(TABLE_SIZE), entry_count(0) {}

// Copy constructor - Deep copy of all elements
table::table(const table& other) : t(TABLE_SIZE), entry_count(other.entry_count) {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (other.t[i]) {
            t[i] = ::make_unique<kvp>(*other.t[i]);
        }
    }
}

// Copy assignment operator - Deep copy with proper cleanup
table& table::operator=(const table& other) {
    if (this != &other) {
        t.clear();
        t.resize(TABLE_SIZE);
        entry_count = other.entry_count;
        
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            if (other.t[i]) {
                t[i] = ::make_unique<kvp>(*other.t[i]);
            }
        }
    }
    return *this;
}

// Get value by key - optimized with noexcept
std::pair<bool, std::bitset<5>> table::get(char key) const noexcept {
    int pos = findPosition(key);
    if (pos == -1 || !t[pos]) {
        return std::make_pair(false, std::bitset<5>());
    }
    return std::make_pair(true, t[pos]->value);
}

// Get key by value - optimized with const reference and noexcept
std::pair<bool, char> table::get(const std::bitset<5>& value) const noexcept {
    // Early exit for empty table
    if (isEmpty()) {
        return std::make_pair(false, '\0');
    }
    
    // Direct lookup for small tables, find_if for larger ones
    if (entry_count < 16) {
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            if (t[i] && t[i]->value == value) {
                return std::make_pair(true, t[i]->key);
            }
        }
    } else {
        // C++11 compatible version - specify the exact type for lambda parameter
        typedef std::unique_ptr<kvp> kvp_ptr;
        auto it = std::find_if(t.begin(), t.end(),
            [&value](const kvp_ptr& entry) -> bool {
                return entry && entry->value == value;
            });
        
        if (it != t.end()) {
            return std::make_pair(true, (*it)->key);
        }
    }
    
    return std::make_pair(false, '\0');
}

// Overload to get directly to a letter
std::pair<bool, letter> table::get_letter(char key) const noexcept {
    int pos = findPosition(key);
    if (pos == -1 || !t[pos]) {
        return std::make_pair(false, letter());
    }
    return std::make_pair(true, letter(t[pos]->value));
}

// Insert or update key-value pair - optimized with const reference and noexcept
bool table::put(char key, const std::bitset<5>& val) noexcept {
    // Check if we need to update an existing entry
    int pos = findPosition(key);
    if (pos == -1) {
        return false;  // Table is full
    }
    
    // If position is empty, increment entry count
    if (!t[pos]) {
        entry_count++;
    }
    
    t[pos] = ::make_unique<kvp>(key, val);
    return true;
}

// Fast contains check
bool table::contains(char key) const noexcept {
    int pos = findPosition(key);
    return (pos != -1 && t[pos] && t[pos]->key == key);
}

// Remove an entry
bool table::remove(char key) noexcept {
    int pos = findPosition(key);
    if (pos != -1 && t[pos] && t[pos]->key == key) {
        t[pos].reset();
        entry_count--;
        return true;
    }
    return false;
}

// Clear the table
void table::clear() noexcept {
    for (auto& entry : t) {
        entry.reset();
    }
    entry_count = 0;
}

// Batch operations for better performance
void table::put_batch(const char* keys, const std::bitset<5>* values, size_t count) noexcept {
    if (!keys || !values || count == 0) return;
    
    for (size_t i = 0; i < count; i++) {
        put(keys[i], values[i]);
    }
}

// Print table contents
void table::print() const {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (t[i]) {
            std::cout << i << " : " << t[i]->key << " -> ";
            for (size_t j = 0; j < 5; j++) {
                std::cout << t[i]->value[j];
            }
            std::cout << '\n';
        }
    }
}

// Hash function for characters - optimized with inline and noexcept
inline int table::hash(char c) const noexcept {
    // Using faster bit operations for modulo since TABLE_SIZE is a power of 2
    return (static_cast<unsigned char>(c) * 2654435761U) & TABLE_SIZE_MASK;
}

// Hash function for bitsets - optimized with inline, const reference, and noexcept
inline int table::hash(const std::bitset<5>& b) const noexcept {
    // Using faster bit operations for modulo and better hash formula
    return ((b.to_ulong() * 2654435761U) & TABLE_SIZE_MASK);
}

// Find position for a key using linear probing - optimized with noexcept
int table::findPosition(char key) const noexcept {
    const int start = hash(key);
    int pos = start;
    int probeCount = 0;
    
    // Use a fixed limit to prevent infinite loops
    while (probeCount < (int)TABLE_SIZE) {
        // Early exit condition with cache-friendly check
        if (!t[pos] || t[pos]->key == key) {
            return pos;
        }
        
        // Linear probing with bit mask for faster modulo
        pos = (pos + 1) & TABLE_SIZE_MASK;
        probeCount++;
    }
    
    return -1;  // Table is full
}

// Find position using double hashing (more efficient) - not used by default but available
int table::findPositionDoubleHash(char key) const noexcept {
    const int h1 = hash(key);
    // Different secondary hash function to avoid clustering
    const int h2 = 1 + (static_cast<unsigned char>(key) % (TABLE_SIZE - 1));
    
    int pos = h1;
    int probeCount = 0;
    
    while (probeCount < (int)TABLE_SIZE) {
        if (!t[pos] || t[pos]->key == key) {
            return pos;
        }
        
        // Double hashing formula
        pos = (pos + h2) & TABLE_SIZE_MASK;
        probeCount++;
    }
    
    return -1;  // Table is full
}
