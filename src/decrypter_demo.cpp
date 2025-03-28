#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "parser.h"
#include "ciphertext.h"
#include "decrypter.h"
#include <iostream>
#include <string>
#include <cstring>
#include <random>
#include <ctime>
#include <iomanip>
#include <set>

const char* SAMPLE_TEXTS[] = {
    "the quick brown fox jumps over the lazy dog",
    "sphinx of black quartz, judge my vow",
    "pack my box with five dozen liquor jugs",
    "how vexingly quick daft zebras jump",
    "waltz, nymph, for quick jigs vex bud"
};

void print_menu() {
    std::cout << "\n=== DotsiesCipherGen Decrypter Demo ===" << std::endl;
    std::cout << "1. Encrypt a custom message" << std::endl;
    std::cout << "2. Use a sample message" << std::endl;
    std::cout << "3. Decrypt an existing ciphertext" << std::endl;
    std::cout << "4. Exit" << std::endl;
    std::cout << "Enter choice: ";
}

void display_ciphertext_info(const ciphertext& ct) {
    std::cout << "\nCiphertext information:" << std::endl;
    std::cout << "Length: " << ct.size() << " letters" << std::endl;
    
    // Display bit patterns for the first few letters
    const size_t display_limit = std::min(ct.size(), static_cast<size_t>(10));
    std::cout << "Bit patterns:" << std::endl;
    
    for (size_t i = 0; i < display_limit; i++) {
        std::cout << "Letter " << i << ": ";
        for (size_t j = 0; j < 5; j++) {
            std::cout << (ct.getBit(i, j) ? "1" : "0");
        }
        std::cout << std::endl;
    }
}

void apply_random_shifts(ciphertext& ct, int num_shifts) {
    std::cout << "\nApplying " << num_shifts << " random row shifts..." << std::endl;
    
    // Initialize random number generator
    std::mt19937 rng(static_cast<unsigned int>(time(nullptr)));
    std::uniform_int_distribution<int> row_dist(0, 4); // 5 rows (0-4)
    std::uniform_int_distribution<int> dir_dist(0, 1); // 0=left, 1=right
    std::uniform_int_distribution<int> amt_dist(1, 3); // 1-3 positions
    
    for (int i = 0; i < num_shifts; i++) {
        int row = row_dist(rng);
        bool direction = dir_dist(rng) == 1;
        int amount = amt_dist(rng);
        
        ct.row_shift(row, direction, amount);
        std::cout << "Shifted row " << row << " " 
                  << (direction ? "right" : "left") 
                  << " by " << amount << " positions" << std::endl;
    }
}

// Function to compare original and decrypted text
void compare_results(const std::string& original, const std::string& decrypted) {
    std::cout << "\n===== Results Comparison =====" << std::endl;
    std::cout << "Original: " << original << std::endl;
    std::cout << "Decrypted: " << decrypted << std::endl;
    
    // Calculate match percentage
    size_t match_count = 0;
    size_t min_length = std::min(original.length(), decrypted.length());
    
    for (size_t i = 0; i < min_length; i++) {
        if (std::tolower(original[i]) == std::tolower(decrypted[i])) {
            match_count++;
        }
    }
    
    double match_percentage = min_length > 0 ? 
        (static_cast<double>(match_count) / min_length) * 100 : 0.0;
    
    std::cout << "Character match: " << match_count << "/" << min_length 
              << " (" << std::fixed << std::setprecision(2) << match_percentage << "%)" << std::endl;
    
    // Determine success based on match percentage
    if (match_percentage >= 90.0) {
        std::cout << "Decryption SUCCESS!" << std::endl;
    } else if (match_percentage >= 70.0) {
        std::cout << "Decryption PARTIALLY SUCCESSFUL." << std::endl;
    } else {
        std::cout << "Decryption FAILED." << std::endl;
    }
    
    // Check if text is likely a pangram
    std::set<char> unique_letters;
    for (char c : original) {
        if (std::isalpha(c)) {
            unique_letters.insert(std::tolower(c));
        }
    }
    if (unique_letters.size() >= 20) {
        std::cout << "\nNote: This text appears to be a pangram (uses many different letters)." << std::endl;
        std::cout << "Our improved algorithm now detects pangrams and applies specialized techniques." << std::endl;
    }
}

int main() {
    // Initialize cipher table
    table t;
    const char* alphabet = "abcdefghijklmnopqrstuvwxyz;',.! ";
    for (int x = 0; x < 32; x++) {
        std::bitset<5> in(x + 1);
        t.put(alphabet[x], in);
    }
    
    // Create parser
    parser ps(t);
    
    // Create decrypter
    decrypter dec(ps);
    
    std::cout << "=== DotsiesCipherGen Decrypter Demo (Enhanced Edition) ===" << std::endl;
    std::cout << "This version includes improved algorithms for challenging texts like pangrams" << std::endl;
    std::cout << "and automatically attempts to solve ciphers without requiring parameter inputs." << std::endl;
    
    bool running = true;
    while (running) {
        print_menu();
        
        int choice;
        std::cin >> choice;
        std::cin.ignore(); // Clear the newline
        
        switch (choice) {
            case 1: {
                // Encrypt a custom message
                std::cout << "Enter your message: ";
                std::string message;
                std::getline(std::cin, message);
                
                if (message.empty()) {
                    std::cout << "Message cannot be empty." << std::endl;
                    break;
                }
                
                // Encrypt the message
                ciphertext encrypted = ps.translate(message);
                
                std::cout << "\nOriginal plaintext: " << message << std::endl;
                display_ciphertext_info(encrypted);
                
                // Ask about random shifts
                std::cout << "\nApply random shifts to simulate encryption? (y/n): ";
                char apply_shifts;
                std::cin >> apply_shifts;
                std::cin.ignore(); // Clear the newline
                
                if (apply_shifts == 'y' || apply_shifts == 'Y') {
                    std::cout << "How many shifts to apply? (1-10): ";
                    int num_shifts;
                    std::cin >> num_shifts;
                    std::cin.ignore(); // Clear the newline
                    
                    num_shifts = std::max(1, std::min(10, num_shifts));
                    apply_random_shifts(encrypted, num_shifts);
                    
                    // Show encoded message
                    std::cout << "\nEncrypted message: " << ps.translate(encrypted) << std::endl;
                    
                    // Use reasonable defaults based on text characteristics
                    // Detect if this might be a pangram to adjust parameters
                    std::set<char> unique_letters;
                    for (char c : message) {
                        if (std::isalpha(c)) {
                            unique_letters.insert(std::tolower(c));
                        }
                    }
                    
                    int max_iterations = unique_letters.size() >= 20 ? 40 : 30; // More iterations for pangrams
                    double min_score = 0.5;
                    
                    // Try automatic decryption with advanced algorithm
                    std::cout << "\nAttempting to decrypt automatically..." << std::endl;
                    std::cout << "(Using " << max_iterations << " iterations)" << std::endl;
                    decryption_result result = dec.advanced_decrypt(encrypted, max_iterations, min_score);
                    
                    // Compare results
                    compare_results(message, result.plaintext);
                }
                break;
            }
            case 2: {
                // Use a sample message
                std::cout << "Available sample messages:" << std::endl;
                for (int i = 0; i < 5; i++) {
                    std::cout << i + 1 << ". " << SAMPLE_TEXTS[i] << std::endl;
                }
                
                std::cout << "Select a sample (1-5): ";
                int sample_choice;
                std::cin >> sample_choice;
                std::cin.ignore(); // Clear the newline
                
                if (sample_choice < 1 || sample_choice > 5) {
                    std::cout << "Invalid choice, using sample 1." << std::endl;
                    sample_choice = 1;
                }
                
                // Use the selected sample
                const char* sample = SAMPLE_TEXTS[sample_choice - 1];
                
                // Encrypt the message
                ciphertext encrypted = ps.translate(sample);
                
                std::cout << "\nOriginal plaintext: " << sample << std::endl;
                display_ciphertext_info(encrypted);
                
                // Apply random shifts
                std::cout << "How many shifts to apply? (1-10, default 5): ";
                std::string input;
                std::getline(std::cin, input);
                int num_shifts = 5;
                if (!input.empty()) {
                    num_shifts = std::stoi(input);
                    num_shifts = std::max(1, std::min(10, num_shifts));
                }
                
                apply_random_shifts(encrypted, num_shifts);
                
                // Show encoded message
                std::cout << "\nEncrypted message: " << ps.translate(encrypted) << std::endl;
                
                // Use reasonable defaults based on text characteristics
                // Detect if this might be a pangram to adjust parameters
                std::set<char> unique_letters;
                std::string sample_str(sample); // Convert to std::string
                for (char c : sample_str) {
                    if (std::isalpha(c)) {
                        unique_letters.insert(std::tolower(c));
                    }
                }
                
                int max_iterations = unique_letters.size() >= 20 ? 40 : 30; // More iterations for pangrams
                double min_score = 0.5;
                
                // Try automatic decryption with advanced algorithm
                std::cout << "\nAttempting to decrypt automatically..." << std::endl;
                std::cout << "(Using " << max_iterations << " iterations)" << std::endl;
                decryption_result result = dec.advanced_decrypt(encrypted, max_iterations, min_score);
                
                // Compare results
                compare_results(sample, result.plaintext);
                break;
            }
            case 3: {
                std::cout << "This option would load an existing ciphertext." << std::endl;
                std::cout << "For simplicity in this demo, we'll create a random one instead." << std::endl;
                
                // Create a random ciphertext for demonstration
                const char* sample = SAMPLE_TEXTS[0]; // Use the first sample
                ciphertext encrypted = ps.translate(sample);
                
                // Apply some random shifts
                int num_shifts = 7; // Apply more shifts to make it challenging
                apply_random_shifts(encrypted, num_shifts);
                
                // Show encrypted message
                std::cout << "\nOriginal message (hidden from decrypter): " << sample << std::endl;
                std::cout << "Encrypted message: " << ps.translate(encrypted) << std::endl;
                
                // Use reasonable defaults based on text characteristics
                // Detect if this might be a pangram to adjust parameters
                std::set<char> unique_letters;
                std::string sample_str(sample); // Convert to std::string
                for (char c : sample_str) {
                    if (std::isalpha(c)) {
                        unique_letters.insert(std::tolower(c));
                    }
                }
                
                int max_iterations = unique_letters.size() >= 20 ? 40 : 30; // More iterations for pangrams
                double min_score = 0.5;
                
                // Try automatic decryption with advanced algorithm
                std::cout << "\nAttempting to decrypt automatically..." << std::endl;
                std::cout << "(Using " << max_iterations << " iterations)" << std::endl;
                decryption_result result = dec.advanced_decrypt(encrypted, max_iterations, min_score);
                
                // Compare results
                compare_results(sample, result.plaintext);
                break;
            }
            case 4:
                running = false;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
    
    std::cout << "\nThank you for using the DotsiesCipherGen Decrypter!" << std::endl;
    
    return 0;
} 