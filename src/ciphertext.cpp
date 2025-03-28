#include "ciphertext.h"
#include "table.h"
#include <iostream>
#include <iomanip>

// Utility
void ciphertext::print() const {
    // Check if ciphertext is empty
    if (phrase.empty()) {
        std::cout << "Empty ciphertext" << std::endl;
        return;
    }
    
    // Print header with size information
    std::cout << "Ciphertext with " << phrase.size() << " letters:" << std::endl;
    
    // Calculate row count based on display width
    const size_t WIDTH = 10; // Letters per row
    const size_t rows = (phrase.size() + WIDTH - 1) / WIDTH;
    
    // Print column headers
    std::cout << "    ";
    for (size_t i = 0; i < std::min(WIDTH, phrase.size()); ++i) {
        std::cout << std::setw(2) << i << " ";
    }
    std::cout << std::endl;
    
    // Print horizontal separator
    std::cout << "    ";
    for (size_t i = 0; i < std::min(WIDTH, phrase.size()); ++i) {
        std::cout << "---";
    }
    std::cout << std::endl;
    
    // Print each row
    for (size_t row = 0; row < rows; ++row) {
        // Print row header
        std::cout << std::setw(2) << row << " |";
        
        // Calculate range for this row
        const size_t start = row * WIDTH;
        const size_t end = std::min(start + WIDTH, phrase.size());
        
        // Print each letter in this row
        for (size_t i = start; i < end; ++i) {
            std::cout << " ";
            // Display each bit in the letter
            for (size_t bit = 0; bit < 5; ++bit) {
                std::cout << (phrase[i].b[bit] ? "#" : " ");
            }
        }
        std::cout << std::endl;
    }
}

// Implementation of from_string method
ciphertext ciphertext::from_string(const std::string& str, const table& t) {
    ciphertext result(str.size());
    for (std::size_t i = 0; i < str.size(); ++i) {
        std::pair<bool, std::bitset<5>> res = t.get(str[i]);
        result.set(i, letter(res.second));
    }
    return result;
}
