#include "table.h"
#include <stdexcept>
#include <algorithm>
#include <iostream>

// Constructor - Initialize empty table
table::table() : t(TABLE_SIZE) {}

// Copy constructor - Deep copy of all elements
table::table(const table& other) : t(TABLE_SIZE) {
    for (size_t i = 0; i < TABLE_SIZE; i++) {
        if (other.t[i]) {
            t[i] = std::make_unique<kvp>(*other.t[i]);
        }
    }
}

// Copy assignment operator - Deep copy with proper cleanup
table& table::operator=(const table& other) {
    if (this != &other) {
        t.clear();
        t.resize(TABLE_SIZE);
        for (size_t i = 0; i < TABLE_SIZE; i++) {
            if (other.t[i]) {
                t[i] = std::make_unique<kvp>(*other.t[i]);
            }
        }
    }
    return *this;
}

// Get value by key
std::pair<bool, bitset<5>> table::get(char key) const {
    int pos = findPosition(key);
    if (pos == -1 || !t[pos]) {
        return {false, bitset<5>()};
    }
    return {true, t[pos]->value};
}

// Get key by value
std::pair<bool, char> table::get(bitset<5> value) const {
    auto it = std::find_if(t.begin(), t.end(),
        [&value](const auto& entry) {
            return entry && entry->value == value;
        });
    
    if (it != t.end()) {
        return {true, (*it)->key};
    }
    return {false, '\0'};
}

// Insert or update key-value pair
bool table::put(char key, bitset<5> val) {
    int pos = findPosition(key);
    if (pos == -1) {
        return false;  // Table is full
    }
    
    t[pos] = std::make_unique<kvp>(key, val);
    return true;
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

// Private helper methods

// Hash function for characters
int table::hash(char c) const {
    return ((static_cast<int>(c) * 31) % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
}

// Hash function for bitsets
int table::hash(bitset<5> b) const {
    return ((static_cast<int>(b.to_ulong()) * 31) % TABLE_SIZE + TABLE_SIZE) % TABLE_SIZE;
}

// Find position for a key using linear probing
int table::findPosition(char key) const {
    int start = hash(key);
    int pos = start;
    int probeCount = 0;
    
    while (probeCount < TABLE_SIZE) {
        if (!t[pos] || t[pos]->key == key) {
            return pos;
        }
        pos = (pos + 1) % TABLE_SIZE;
        probeCount++;
    }
    
    return -1;  // Table is full
}
