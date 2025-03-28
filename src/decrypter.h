#ifndef DECRYPTER_H_
#define DECRYPTER_H_

#include "base.h"
#include "ciphertext.h"
#include "table.h"
#include "parser.h"
#include <string>
#include <vector>
#include <map>
#include <utility>
#include <array>
#include <algorithm>
#include <functional>
#include <iostream>
#include <unordered_map>

// Structure to hold a potential decryption attempt with its score
struct decryption_result {
    ciphertext cipher;
    std::string plaintext;
    double score;
    
    // Default constructor
    decryption_result() : cipher(), plaintext(""), score(0.0) {}
    
    // Constructor
    decryption_result(const ciphertext& c, const std::string& p, double s)
        : cipher(c), plaintext(p), score(s) {}
    
    // Allow sorting by score (highest first)
    bool operator<(const decryption_result& other) const {
        return score > other.score; // Note: reversed for descending sort
    }
};

class decrypter {
private:
    // Reference to the parser and table for translation
    const parser& ps;
    
    // Dictionary for word lookup and scoring
    std::vector<std::string> dictionary;
    
    // N-gram frequency tables
    std::unordered_map<std::string, double> bigrams;
    std::unordered_map<std::string, double> trigrams;
    
    // English letter frequency table (from most to least common)
    static const std::array<char, 26> LETTER_FREQUENCY;
    
    // Frequency scoring
    double score_text(const std::string& text) const;
    
    // Dictionary scoring
    double score_by_dictionary(const std::string& text) const;
    
    // N-gram scoring
    double score_by_ngrams(const std::string& text) const;
    
    // Combined advanced scoring
    double score_advanced(const std::string& text) const;
    
    // Try all combinations of row shifts
    std::vector<decryption_result> try_all_row_shifts(const ciphertext& original, 
                                                      int max_shifts_per_row = 2) const;
    
    // Try shift patterns based on common words
    std::vector<decryption_result> try_pattern_matching(const ciphertext& original) const;
    
    // Check for recognizable patterns
    bool has_recognizable_pattern(const ciphertext& c) const;
    
    // Initialize dictionary from a file
    void load_dictionary(const std::string& filename);
    
    // Initialize n-gram data
    void initialize_ngrams();
    
    // Simulated annealing optimizer
    decryption_result simulated_annealing(const ciphertext& original, 
                                         int iterations = 2500, 
                                         double initial_temp = 10.0) const;
    
    // Beam search optimizer
    std::vector<decryption_result> beam_search(const ciphertext& original, 
                                              int beam_width = 8,
                                              int iterations = 250) const;
    
    // Word pattern analysis
    std::vector<std::pair<std::string, size_t>> extract_patterns(const std::string& text) const;
    bool patterns_match(const std::vector<std::pair<std::string, size_t>>& pattern1,
                       const std::vector<std::pair<std::string, size_t>>& pattern2) const;

public:
    // Constructor
    decrypter(const parser& parser_ref);
    
    // Load dictionary from file
    decrypter(const parser& parser_ref, const std::string& dictionary_file);
    
    // Attempt to decrypt a ciphertext
    std::vector<decryption_result> decrypt(const ciphertext& encrypted) const;
    
    // Attempt to decrypt with constraints (e.g., known letter positions)
    std::vector<decryption_result> decrypt_with_constraints(
        const ciphertext& encrypted,
        const std::map<size_t, char>& known_positions) const;
    
    // Analyze and display information about a ciphertext
    void analyze(const ciphertext& encrypted) const;
    
    // Get the best guess as a string
    std::string best_guess(const ciphertext& encrypted) const;
    
    // Interactively decrypt
    void interactive_decrypt(ciphertext& encrypted) const;
    
    // Auto decrypt - Try multiple strategies until we find something that looks right
    // Returns the best result found after max_iterations attempts
    decryption_result auto_decrypt(const ciphertext& encrypted, 
                                  int max_iterations = 25,
                                  double min_score_threshold = 0.5) const;
    
    // Improved auto decrypt using advanced techniques
    decryption_result advanced_decrypt(const ciphertext& encrypted,
                                     int max_iterations = 25,
                                     double min_score_threshold = 0.5) const;
};

#endif // DECRYPTER_H_ 