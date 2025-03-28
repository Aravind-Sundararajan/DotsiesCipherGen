#include "parser.h"
#include <cstring>
#include <algorithm>

// C-style string to ciphertext
ciphertext parser::translate(const char* plaintext) const {
    if (!plaintext) return ciphertext(0);
    
    const std::size_t slen = std::strlen(plaintext);
    ciphertext c(slen);
    
    for (std::size_t x = 0; x < slen; ++x) {
        std::pair<bool, std::bitset<5>> result = tab->get(plaintext[x]);
        c.set(x, letter(result.first ? result.second : std::bitset<5>()));
    }
    
    return c;
}

// std::string to ciphertext
ciphertext parser::translate(const std::string& plaintext) const {
    const std::size_t slen = plaintext.size();
    ciphertext c(slen);
    
    for (std::size_t x = 0; x < slen; ++x) {
        std::pair<bool, std::bitset<5>> result = tab->get(plaintext[x]);
        c.set(x, letter(result.first ? result.second : std::bitset<5>()));
    }
    
    return c;
}

// Translate ciphertext to C-style string
void parser::translate(const ciphertext& c, char* oStr) const {
    if (!oStr) return;
    
    const std::size_t size = c.size();
    for (std::size_t x = 0; x < size; ++x) {
        std::pair<bool, char> result = tab->get(c.get(x).b);
        oStr[x] = result.first ? result.second : ' ';
    }
    oStr[size] = '\0'; // Null terminator
}

// Translate ciphertext to std::string
std::string parser::translate(const ciphertext& c) const {
    const std::size_t size = c.size();
    std::string result_str;
    result_str.reserve(size); // Pre-allocate to avoid reallocations
    
    for (std::size_t x = 0; x < size; ++x) {
        std::pair<bool, char> result = tab->get(c.get(x).b);
        result_str.push_back(result.first ? result.second : ' ');
    }
    
    return result_str;
}

// Batch processing for better cache locality
ciphertext parser::translate_batch(const char* plaintext, size_t length, size_t batch_size) const {
    if (!plaintext || length == 0) return ciphertext(0);
    
    ciphertext c(length);
    const size_t MAX_BATCH = 128; // Maximum reasonable batch size
    
    // Use a reasonable batch size
    batch_size = std::min(batch_size, MAX_BATCH);
    batch_size = std::min(batch_size, length);
    
    // Process in batches for better cache efficiency
    for (std::size_t pos = 0; pos < length; pos += batch_size) {
        const std::size_t current_batch = std::min(batch_size, length - pos);
        
        // Process a batch of characters
        for (std::size_t i = 0; i < current_batch; ++i) {
            const std::size_t idx = pos + i;
            std::pair<bool, std::bitset<5>> result = tab->get(plaintext[idx]);
            c.set(idx, letter(result.first ? result.second : std::bitset<5>()));
        }
    }
    
    return c;
}

// Cache-optimized translation with pre-reservation
ciphertext parser::translate_optimized(const std::string& plaintext) const {
    const std::size_t slen = plaintext.size();
    if (slen == 0) return ciphertext(0);
    
    ciphertext c(slen);
    
    // For large texts, we use batch processing with prefetching
    if (slen > 64) {
        const size_t CACHE_LINE = 64; // Typical cache line size
        
        for (std::size_t i = 0; i < slen; i += CACHE_LINE) {
            // Process a cache line worth of characters
            const std::size_t end = std::min(i + CACHE_LINE, slen);
            
            for (std::size_t j = i; j < end; ++j) {
                std::pair<bool, std::bitset<5>> result = tab->get(plaintext[j]);
                c.set(j, letter(result.first ? result.second : std::bitset<5>()));
            }
        }
    } else {
        // For small texts, simple loop is fine
        for (std::size_t x = 0; x < slen; ++x) {
            std::pair<bool, std::bitset<5>> result = tab->get(plaintext[x]);
            c.set(x, letter(result.first ? result.second : std::bitset<5>()));
        }
    }
    
    return c;
}
