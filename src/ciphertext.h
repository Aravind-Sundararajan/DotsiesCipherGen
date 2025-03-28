#ifndef CIP_H_
#define CIP_H_

#include "base.h"
#include "letter.h"
#include <vector>
#include <memory>
#include <algorithm>

class table; // Forward declaration for table reference

class ciphertext {
public:
    // Constructors and destructor
    ciphertext() noexcept : phrase(1) {}
    explicit ciphertext(std::size_t s) noexcept : phrase(s) {}
    ~ciphertext() = default;
    
    // Copy operations
    ciphertext(const ciphertext& other) = default;
    ciphertext& operator=(const ciphertext& other) = default;
    
    // Move operations
    ciphertext(ciphertext&& other) noexcept = default;
    ciphertext& operator=(ciphertext&& other) noexcept = default;

    // Core operations - with bounds checking in debug builds
    letter& operator()(std::size_t x) {
        #ifdef NDEBUG
            return phrase[x];
        #else
            return at(x);
        #endif
    }
    
    const letter& operator()(std::size_t x) const {
        #ifdef NDEBUG
            return phrase[x];
        #else
            return at(x);
        #endif
    }
    
    letter& get(std::size_t x) {
        return operator()(x);
    }
    
    const letter& get(std::size_t x) const {
        return operator()(x);
    }
    
    // Safe accessor with bounds checking
    letter& at(std::size_t x) {
        if (x >= phrase.size()) {
            throw std::out_of_range("ciphertext index out of range");
        }
        return phrase[x];
    }
    
    const letter& at(std::size_t x) const {
        if (x >= phrase.size()) {
            throw std::out_of_range("ciphertext index out of range");
        }
        return phrase[x];
    }
    
    void set(std::size_t x, const letter& l) {
        #ifdef NDEBUG
            phrase[x] = l;
        #else
            at(x) = l;
        #endif
    }
    
    // Efficient append methods
    ciphertext& append_inplace(const letter& l) {
        phrase.push_back(l);
        return *this;
    }
    
    ciphertext append(const letter& l) const {
        ciphertext result(*this);
        result.append_inplace(l);
        return result;
    }
    
    // Set all elements from another ciphertext
    void set_all(const ciphertext& c) {
        const std::size_t copy_size = std::min(size(), c.size());
        for (std::size_t x = 0; x < copy_size; x++) {
            phrase[x] = c.phrase[x];
        }
    }

    // Transformations - all with bounds checking in non-optimized builds
    void row_swap(size_t p1, size_t p2) noexcept {
        if (p1 >= 5 || p2 >= 5 || p1 == p2) return;
        
        for (auto& letter : phrase) {
            letter.swap(p1, p2);
        }
    }
    
    void column_swap(size_t p1, size_t p2) noexcept {
        if (p1 >= size() || p2 >= size() || p1 == p2) return;
        std::swap(phrase[p1], phrase[p2]);
    }
    
    // Optimized column shift methods
    void column_shift() noexcept {
        for (auto& letter : phrase) {
            letter.circle_shift();
        }
    }
    
    void column_shift(bool dir) noexcept {
        for (auto& letter : phrase) {
            letter.circle_shift(dir);
        }
    }
    
    void column_shift(bool dir, int amount) noexcept {
        if (amount == 0) return;
        
        // Optimize by using modulo
        amount = amount % 5;
        if (amount == 0) return;
        
        for (auto& letter : phrase) {
            letter.circle_shift(dir, amount);
        }
    }
    
    // Optimized row shift methods
    void row_shift(std::size_t r) noexcept {
        row_shift(r, true); // Default direction is up/right
    }
    
    void row_shift(std::size_t r, bool direction) noexcept {
        if (empty() || r >= 5) return;
        
        const std::size_t len = size();
        if (len <= 1) return; // No shift needed for single letter
        
        if (direction) {
            // Shift up/right
            bool temp = phrase[0].b[r];
            for (std::size_t x = 0; x < len - 1; x++) {
                phrase[x].b[r] = phrase[x + 1].b[r];
            }
            phrase[len - 1].b[r] = temp;
        } else {
            // Shift down/left
            bool temp = phrase[len - 1].b[r];
            for (std::size_t x = len - 1; x > 0; x--) {
                phrase[x].b[r] = phrase[x - 1].b[r];
            }
            phrase[0].b[r] = temp;
        }
    }
    
    // Optimized row shift by amount
    void row_shift(std::size_t r, bool direction, int amount) noexcept {
        if (empty() || r >= 5 || amount == 0) return;
        
        const std::size_t len = size();
        if (len <= 1) return; // No shift needed for single letter
        
        // Optimize by using modulo
        amount = amount % len;
        if (amount == 0) return;
        
        // Create a temporary array for the bit values of row r
        std::vector<bool> temp(len);
        for (std::size_t i = 0; i < len; i++) {
            temp[i] = phrase[i].b[r];
        }
        
        // Apply the shift
        for (std::size_t i = 0; i < len; i++) {
            std::size_t src_idx;
            if (direction) {
                // Right shift
                src_idx = (i + amount) % len;
            } else {
                // Left shift
                src_idx = (i + len - amount) % len;
            }
            phrase[i].b[r] = temp[src_idx];
        }
    }

    // Optimized bit access method with optional bounds checking
    bool getBit(std::size_t letterIndex, std::size_t bitIndex) const noexcept {
        #ifdef NDEBUG
            return phrase[letterIndex].b[bitIndex];
        #else
            return (letterIndex < phrase.size() && bitIndex < 5) ? 
                phrase[letterIndex].b[bitIndex] : false;
        #endif
    }
    
    void setBit(std::size_t letterIndex, std::size_t bitIndex, bool value) noexcept {
        #ifdef NDEBUG
            phrase[letterIndex].b[bitIndex] = value;
        #else
            if (letterIndex < phrase.size() && bitIndex < 5) {
                phrase[letterIndex].b[bitIndex] = value;
            }
        #endif
    }
    
    // Construction from string
    static ciphertext from_string(const std::string& str, const table& t);
    
    // Batch operations for better performance
    void set_batch(std::size_t start_idx, const std::vector<letter>& letters) noexcept {
        if (start_idx >= phrase.size()) return;
        
        const std::size_t copy_count = std::min(letters.size(), phrase.size() - start_idx);
        for (std::size_t i = 0; i < copy_count; i++) {
            phrase[start_idx + i] = letters[i];
        }
    }

    // Utility methods
    void print() const;
    std::size_t size() const noexcept { return phrase.size(); }
    bool empty() const noexcept { return phrase.empty(); }
    
    // Resizing operations
    void resize(std::size_t new_size) {
        phrase.resize(new_size);
    }
    
    void reserve(std::size_t capacity) {
        phrase.reserve(capacity);
    }
    
    std::size_t capacity() const noexcept {
        return phrase.capacity();
    }
    
    // Range-based for loop support
    typename std::vector<letter>::iterator begin() noexcept { return phrase.begin(); }
    typename std::vector<letter>::iterator end() noexcept { return phrase.end(); }
    typename std::vector<letter>::const_iterator begin() const noexcept { return phrase.begin(); }
    typename std::vector<letter>::const_iterator end() const noexcept { return phrase.end(); }
    typename std::vector<letter>::const_iterator cbegin() const noexcept { return phrase.cbegin(); }
    typename std::vector<letter>::const_iterator cend() const noexcept { return phrase.cend(); }

private:
    std::vector<letter> phrase;
};

#endif // CIP_H_
