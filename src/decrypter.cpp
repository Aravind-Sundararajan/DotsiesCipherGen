#include "decrypter.h"
#include <fstream>
#include <sstream>
#include <cstring>
#include <cctype>
#include <cmath>
#include <set>
#include <limits>
#include <random>
#include <chrono>
#include <iomanip> // For std::setprecision

// Initialize letter frequency table (most common English letters)
const std::array<char, 26> decrypter::LETTER_FREQUENCY = {
    'e', 't', 'a', 'o', 'i', 'n', 's', 'h', 'r', 'd', 
    'l', 'c', 'u', 'm', 'w', 'f', 'g', 'y', 'p', 'b', 
    'v', 'k', 'j', 'x', 'q', 'z'
};

// Constructor
decrypter::decrypter(const parser& parser_ref) 
    : ps(parser_ref) {
    // Initialize with some common English words for pattern matching
    dictionary = {
        "the", "and", "that", "have", "for", "not", "with", "you", "this", 
        "but", "his", "from", "they", "say", "her", "she", "will", "one", 
        "all", "would", "there", "their", "what", "out", "about", "who", 
        "get", "which", "when", "make", "can", "like", "time", "just", 
        "him", "know", "take", "people", "into", "year", "your", "good", 
        "some", "could", "them", "see", "other", "than", "then", "now", 
        "look", "only", "come", "its", "over", "think", "also", "back", 
        "after", "use", "two", "how", "our", "work", "first", "well"
    };
    
    // Initialize n-gram data
    initialize_ngrams();
}

// Constructor with dictionary file
decrypter::decrypter(const parser& parser_ref, const std::string& dictionary_file) 
    : ps(parser_ref) {
    load_dictionary(dictionary_file);
    initialize_ngrams();
}

// Initialize n-gram frequency data
void decrypter::initialize_ngrams() {
    // Common English bigrams with approximate frequencies
    bigrams = {
        {"th", 3.56}, {"he", 3.07}, {"in", 2.43}, {"er", 2.05}, {"an", 1.99},
        {"re", 1.85}, {"on", 1.76}, {"at", 1.49}, {"en", 1.45}, {"nd", 1.35},
        {"ti", 1.34}, {"es", 1.34}, {"or", 1.28}, {"te", 1.20}, {"of", 1.17},
        {"ed", 1.17}, {"is", 1.13}, {"it", 1.12}, {"al", 1.09}, {"ar", 1.07},
        {"st", 1.05}, {"to", 1.04}, {"nt", 1.04}, {"ng", 0.95}, {"se", 0.93},
        {"ha", 0.93}, {"as", 0.87}, {"ou", 0.87}, {"io", 0.83}, {"le", 0.83},
        {"ve", 0.83}, {"co", 0.79}, {"me", 0.79}, {"de", 0.76}, {"hi", 0.76},
        {"ri", 0.73}, {"ro", 0.73}, {"ic", 0.70}, {"ne", 0.69}, {"ea", 0.69},
        {"ra", 0.69}, {"ce", 0.68}, {"li", 0.62}, {"ch", 0.60}, {"ll", 0.58},
        {"be", 0.58}, {"ma", 0.56}, {"si", 0.55}, {"om", 0.55}, {"ur", 0.54}
    };
    
    // Common English trigrams with approximate frequencies
    trigrams = {
        {"the", 3.51}, {"and", 1.59}, {"ing", 1.15}, {"her", 0.82}, {"hat", 0.65},
        {"his", 0.65}, {"tha", 0.64}, {"ere", 0.59}, {"for", 0.58}, {"ent", 0.55},
        {"ion", 0.55}, {"ter", 0.54}, {"was", 0.52}, {"you", 0.51}, {"ith", 0.50},
        {"ver", 0.50}, {"all", 0.49}, {"wit", 0.44}, {"thi", 0.44}, {"tio", 0.43},
        {"nde", 0.39}, {"ave", 0.39}, {"res", 0.38}, {"pow", 0.38}, {"oth", 0.38}
    };
}

// Load dictionary from file
void decrypter::load_dictionary(const std::string& filename) {
    std::ifstream file(filename);
    if (file.is_open()) {
        std::string line;
        while (std::getline(file, line)) {
            // Remove any trailing whitespace
            line.erase(line.find_last_not_of(" \n\r\t") + 1);
            if (!line.empty()) {
                dictionary.push_back(line);
            }
        }
        file.close();
    } else {
        std::cerr << "Warning: Could not open dictionary file: " << filename << std::endl;
        // Fall back to common words
        dictionary = {
            "the", "and", "that", "have", "for", "not", "with", "you", "this", 
            "but", "his", "from", "they", "say", "her", "she", "will", "one"
        };
    }
}

// Score text based on letter frequency
double decrypter::score_text(const std::string& text) const {
    if (text.empty()) return 0.0;
    
    // Count frequency of each letter
    std::array<int, 26> counts = {0};
    int total_letters = 0;
    
    for (char c : text) {
        if (std::isalpha(c)) {
            counts[std::tolower(c) - 'a']++;
            total_letters++;
        }
    }
    
    if (total_letters == 0) return 0.0;
    
    // Calculate score based on expected letter frequency
    double score = 0.0;
    for (size_t i = 0; i < 26; i++) {
        char expected_char = LETTER_FREQUENCY[i];
        int expected_index = expected_char - 'a';
        
        // Higher score for more common letters appearing with higher frequency
        double expected_weight = 1.0 - (i / 26.0); // Weight from 1.0 to 0.0
        double actual_freq = static_cast<double>(counts[expected_index]) / total_letters;
        
        score += actual_freq * expected_weight;
    }
    
    // Bonus for having spaces in reasonable places
    int space_count = 0;
    for (char c : text) {
        if (c == ' ') space_count++;
    }
    
    // Typical English text has ~15-20% spaces
    double space_ratio = static_cast<double>(space_count) / text.length();
    double ideal_space_ratio = 0.18; // Approximately 1 space per 5-6 characters
    
    // Add bonus for having a reasonable number of spaces
    score += (1.0 - std::abs(space_ratio - ideal_space_ratio)) * 0.5;
    
    return score;
}

// Score based on matching words in the dictionary
double decrypter::score_by_dictionary(const std::string& text) const {
    if (text.empty() || dictionary.empty()) return 0.0;
    
    // Convert to lowercase for comparison
    std::string lower_text = text;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), 
                  [](unsigned char c) { return std::tolower(c); });
    
    // Split into words
    std::istringstream iss(lower_text);
    std::vector<std::string> words;
    std::string word;
    
    while (iss >> word) {
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), 
                  [](char c) { return !std::isalpha(c); }), word.end());
        
        if (!word.empty()) {
            words.push_back(word);
        }
    }
    
    if (words.empty()) return 0.0;
    
    // Count words found in dictionary
    int found_words = 0;
    int total_length = 0;
    
    for (const auto& w : words) {
        if (std::find(dictionary.begin(), dictionary.end(), w) != dictionary.end()) {
            found_words++;
            total_length += w.length();
        }
    }
    
    // Calculate score based on % of words found and their total length
    double word_score = static_cast<double>(found_words) / words.size();
    double length_score = static_cast<double>(total_length) / lower_text.length();
    
    // Combine scores
    return (word_score * 0.6) + (length_score * 0.4);
}

// Score based on n-gram analysis
double decrypter::score_by_ngrams(const std::string& text) const {
    if (text.empty()) return 0.0;
    
    // Convert to lowercase for comparison
    std::string lower_text = text;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), 
                  [](unsigned char c) { return std::tolower(c); });
    
    double bigram_score = 0.0;
    double trigram_score = 0.0;
    
    // Check for bigrams
    size_t bigram_count = 0;
    for (size_t i = 0; i < lower_text.length() - 1; i++) {
        if (!std::isalpha(lower_text[i]) || !std::isalpha(lower_text[i+1])) continue;
        
        std::string bigram = lower_text.substr(i, 2);
        auto it = bigrams.find(bigram);
        if (it != bigrams.end()) {
            bigram_score += it->second;
            bigram_count++;
        }
    }
    
    // Check for trigrams
    size_t trigram_count = 0;
    for (size_t i = 0; i < lower_text.length() - 2; i++) {
        if (!std::isalpha(lower_text[i]) || !std::isalpha(lower_text[i+1]) || !std::isalpha(lower_text[i+2])) continue;
        
        std::string trigram = lower_text.substr(i, 3);
        auto it = trigrams.find(trigram);
        if (it != trigrams.end()) {
            trigram_score += it->second;
            trigram_count++;
        }
    }
    
    // Normalize scores
    double normalized_bigram = (bigram_count > 0) ? 
        bigram_score / static_cast<double>(lower_text.length() - 1) : 0.0;
    double normalized_trigram = (trigram_count > 0) ?
        trigram_score / static_cast<double>(lower_text.length() - 2) : 0.0;
    
    // Combine scores with weights (trigrams are more specific, so higher weight)
    return (normalized_bigram * 0.4) + (normalized_trigram * 0.6);
}

// Combined advanced scoring that integrates multiple scoring methods
double decrypter::score_advanced(const std::string& text) const {
    if (text.empty()) return 0.0;
    
    // Get individual scores
    double freq_score = score_text(text);           // Letter frequency
    double dict_score = score_by_dictionary(text);  // Dictionary word matches
    double ngram_score = score_by_ngrams(text);     // N-gram analysis
    
    // Check for suspicious patterns that suggest garbage text
    bool suspicious = false;
    
    // Check for repeating character patterns (e.g., "ababab")
    if (text.length() >= 6) {
        for (size_t i = 0; i < text.length() - 5; i++) {
            if (text[i] == text[i+2] && text[i] == text[i+4] &&
                text[i+1] == text[i+3] && text[i+1] == text[i+5]) {
                suspicious = true;
                break;
            }
        }
    }
    
    // Check for no spaces in a long text
    if (text.length() > 15) {
        bool has_space = false;
        for (char c : text) {
            if (c == ' ') {
                has_space = true;
                break;
            }
        }
        if (!has_space) suspicious = true;
    }
    
    // Penalize suspicious text
    double suspicion_factor = suspicious ? 0.5 : 1.0;
    
    // Check for pangram-like patterns (text that uses many different letters)
    std::set<char> unique_letters;
    for (char c : text) {
        if (std::isalpha(c)) {
            unique_letters.insert(std::tolower(c));
        }
    }
    
    // Bonus for texts with high letter diversity (pangrams or near-pangrams)
    double unique_ratio = static_cast<double>(unique_letters.size()) / 26.0;
    double letter_diversity_bonus = 0.0;
    
    // If at least 60% of alphabet is used, give a bonus
    if (unique_ratio >= 0.6) {
        letter_diversity_bonus = unique_ratio * 0.2;  // Up to 0.2 bonus for full pangram
    }
    
    // Check for punctuation diversity which is common in pangrams
    std::set<char> punctuation;
    for (char c : text) {
        if (std::ispunct(c)) {
            punctuation.insert(c);
        }
    }
    double punct_bonus = std::min(0.05, static_cast<double>(punctuation.size()) * 0.01);
    
    // Combine scores with weights
    double combined_score = (freq_score * 0.20) + 
                           (dict_score * 0.30) + 
                           (ngram_score * 0.35) +
                           letter_diversity_bonus +
                           punct_bonus;
    
    return combined_score * suspicion_factor;
}

// Try all combinations of row shifts (with a reasonable limit)
std::vector<decryption_result> decrypter::try_all_row_shifts(
    const ciphertext& original, int max_shifts_per_row) const {
    
    std::vector<decryption_result> results;
    
    // Get the size of the ciphertext
    const size_t len = original.size();
    if (len == 0) return results;
    
    // Calculate the maximum number of shifts per row (to avoid combinatorial explosion)
    const int shifts_limit = std::min(max_shifts_per_row, static_cast<int>(len));
    
    // Loop through all combinations of shifts for each row
    // For simplicity, we'll limit the search space
    for (int r0 = 0; r0 <= shifts_limit; r0++) {
        for (int r1 = 0; r1 <= shifts_limit; r1++) {
            for (int r2 = 0; r2 <= shifts_limit; r2++) {
                for (int r3 = 0; r3 <= shifts_limit; r3++) {
                    for (int r4 = 0; r4 <= shifts_limit; r4++) {
                        // Create a copy of the original ciphertext
                        ciphertext test_cipher = original;
                        
                        // Apply shifts
                        if (r0 > 0) test_cipher.row_shift(0, true, r0);
                        if (r1 > 0) test_cipher.row_shift(1, true, r1);
                        if (r2 > 0) test_cipher.row_shift(2, true, r2);
                        if (r3 > 0) test_cipher.row_shift(3, true, r3);
                        if (r4 > 0) test_cipher.row_shift(4, true, r4);
                        
                        // Translate to plaintext
                        std::string plaintext = ps.translate(test_cipher);
                        
                        // Score the result
                        double freq_score = score_text(plaintext);
                        double dict_score = score_by_dictionary(plaintext);
                        
                        // Combine scores (weighted)
                        double combined_score = (freq_score * 0.4) + (dict_score * 0.6);
                        
                        // Add to results
                        results.emplace_back(test_cipher, plaintext, combined_score);
                    }
                }
            }
        }
    }
    
    // Sort by score (highest first)
    std::sort(results.begin(), results.end());
    
    // Limit number of results to avoid overwhelming the user
    const size_t MAX_RESULTS = 10;
    if (results.size() > MAX_RESULTS) {
        results.resize(MAX_RESULTS);
    }
    
    return results;
}

// Try pattern matching based on common words
std::vector<decryption_result> decrypter::try_pattern_matching(
    const ciphertext& original) const {
    
    std::vector<decryption_result> results;
    
    // Get the size of the ciphertext
    const size_t len = original.size();
    if (len == 0) return results;
    
    // Try each common word as a potential pattern
    for (const auto& word : dictionary) {
        // Skip words that are too long for our ciphertext
        if (word.length() > len) continue;
        
        // Skip very short words as they're less useful for pattern matching
        if (word.length() < 3) continue;
        
        // Try placing the word at different positions
        for (size_t pos = 0; pos <= len - word.length(); pos++) {
            // Create a copy of the original ciphertext
            ciphertext test_cipher = original;
            
            // Try different combinations of row shifts
            for (int shifts = 0; shifts < 5; shifts++) {
                // Apply a sequence of shifts to try to match the pattern
                test_cipher.row_shift(shifts % 5, shifts % 2 == 0);
                
                // Translate to plaintext
                std::string plaintext = ps.translate(test_cipher);
                
                // Check if our target word appears at the expected position
                bool pattern_found = true;
                for (size_t i = 0; i < word.length(); i++) {
                    if (std::tolower(plaintext[pos + i]) != std::tolower(word[i])) {
                        pattern_found = false;
                        break;
                    }
                }
                
                if (pattern_found) {
                    // Score the result
                    double freq_score = score_text(plaintext);
                    double dict_score = score_by_dictionary(plaintext);
                    
                    // Boost score since we found a matching pattern
                    double pattern_bonus = 0.5 * (static_cast<double>(word.length()) / len);
                    double combined_score = (freq_score * 0.3) + (dict_score * 0.5) + pattern_bonus;
                    
                    // Add to results
                    results.emplace_back(test_cipher, plaintext, combined_score);
                }
            }
        }
    }
    
    // Sort by score (highest first)
    std::sort(results.begin(), results.end());
    
    // Limit number of results
    const size_t MAX_RESULTS = 10;
    if (results.size() > MAX_RESULTS) {
        results.resize(MAX_RESULTS);
    }
    
    return results;
}

// Check for recognizable patterns in the ciphertext
bool decrypter::has_recognizable_pattern(const ciphertext& c) const {
    // Implementation would check for known patterns or sequences
    // This is a placeholder
    return false;
}

// Main decrypt method
std::vector<decryption_result> decrypter::decrypt(const ciphertext& encrypted) const {
    std::vector<decryption_result> results;
    
    // Try different decryption approaches
    
    // 1. Try brute force row shifts (with reasonable limits)
    auto shift_results = try_all_row_shifts(encrypted);
    results.insert(results.end(), shift_results.begin(), shift_results.end());
    
    // 2. Try pattern matching
    auto pattern_results = try_pattern_matching(encrypted);
    results.insert(results.end(), pattern_results.begin(), pattern_results.end());
    
    // Sort and remove duplicates
    std::sort(results.begin(), results.end());
    results.erase(std::unique(results.begin(), results.end(), 
                [](const decryption_result& a, const decryption_result& b) {
                    return a.plaintext == b.plaintext;
                }), results.end());
    
    // Limit final results
    const size_t MAX_RESULTS = 10;
    if (results.size() > MAX_RESULTS) {
        results.resize(MAX_RESULTS);
    }
    
    return results;
}

// Decrypt with constraints
std::vector<decryption_result> decrypter::decrypt_with_constraints(
    const ciphertext& encrypted,
    const std::map<size_t, char>& known_positions) const {
    
    // Get initial results
    auto results = decrypt(encrypted);
    
    // Filter results that match our constraints
    results.erase(std::remove_if(results.begin(), results.end(),
                 [&known_positions](const decryption_result& r) {
                     for (const auto& pos : known_positions) {
                         if (pos.first >= r.plaintext.size() || 
                             r.plaintext[pos.first] != pos.second) {
                             return true; // Remove this result
                         }
                     }
                     return false; // Keep this result
                 }), results.end());
    
    return results;
}

// Analyze and display information about a ciphertext
void decrypter::analyze(const ciphertext& encrypted) const {
    // 1. Translate the ciphertext to a basic plaintext
    std::string base_plaintext = ps.translate(encrypted);
    
    // 2. Count character frequencies
    std::array<int, 26> char_counts = {0};
    int total_alpha = 0;
    
    for (char c : base_plaintext) {
        if (std::isalpha(c)) {
            char_counts[std::tolower(c) - 'a']++;
            total_alpha++;
        }
    }
    
    // 3. Display analysis information
    std::cout << "======= CIPHERTEXT ANALYSIS =======" << std::endl;
    std::cout << "Length: " << encrypted.size() << " characters" << std::endl;
    std::cout << "Base plaintext: " << base_plaintext << std::endl;
    
    if (total_alpha > 0) {
        std::cout << "\nCharacter frequencies:" << std::endl;
        for (int i = 0; i < 26; i++) {
            if (char_counts[i] > 0) {
                char c = 'a' + i;
                double percentage = (100.0 * char_counts[i]) / total_alpha;
                std::cout << c << ": " << char_counts[i]
                          << " (" << std::fixed << std::setprecision(1) 
                          << percentage << "%)" << std::endl;
            }
        }
    }
    
    // 4. Show encryption pattern analysis
    std::cout << "\nBit pattern visualization:" << std::endl;
    for (size_t i = 0; i < std::min(encrypted.size(), static_cast<size_t>(20)); i++) {
        std::cout << "Letter " << i << ": ";
        for (size_t j = 0; j < 5; j++) {
            std::cout << (encrypted.getBit(i, j) ? "1" : "0");
        }
        std::cout << std::endl;
    }
    
    // 5. Suggest decryption approach
    std::cout << "\nRecommended decryption approach: ";
    if (encrypted.size() < 5) {
        std::cout << "Brute force (short text)" << std::endl;
    } else if (total_alpha > 0 && 
               char_counts['e' - 'a'] + char_counts['t' - 'a'] > total_alpha * 0.15) {
        std::cout << "Frequency analysis (common letters found)" << std::endl;
    } else {
        std::cout << "Pattern matching with dictionary words" << std::endl;
    }
}

// Get the best guess as a string
std::string decrypter::best_guess(const ciphertext& encrypted) const {
    auto results = decrypt(encrypted);
    if (results.empty()) {
        return ps.translate(encrypted); // Return basic translation if no better results
    }
    return results[0].plaintext; // Return highest scoring result
}

// Interactive decryption
void decrypter::interactive_decrypt(ciphertext& encrypted) const {
    // Initial analysis
    analyze(encrypted);
    
    // Get initial translation
    std::string current_plaintext = ps.translate(encrypted);
    std::cout << "\nCurrent plaintext: " << current_plaintext << std::endl;
    
    // Interactive loop
    bool done = false;
    while (!done) {
        std::cout << "\n=== Decryption Menu ===" << std::endl;
        std::cout << "1. Try automatic decryption" << std::endl;
        std::cout << "2. Shift row manually" << std::endl;
        std::cout << "3. Show current plaintext" << std::endl;
        std::cout << "4. Exit" << std::endl;
        std::cout << "Enter choice: ";
        
        int choice;
        std::cin >> choice;
        
        switch (choice) {
            case 1: {
                // Run automatic decryption
                auto results = decrypt(encrypted);
                if (results.empty()) {
                    std::cout << "No decryption candidates found." << std::endl;
                } else {
                    std::cout << "\nTop decryption candidates:" << std::endl;
                    for (size_t i = 0; i < results.size(); i++) {
                        std::cout << i+1 << ". \"" << results[i].plaintext 
                                  << "\" (score: " << results[i].score << ")" << std::endl;
                    }
                    
                    std::cout << "Select a candidate to apply (0 to skip): ";
                    int selection;
                    std::cin >> selection;
                    
                    if (selection > 0 && selection <= static_cast<int>(results.size())) {
                        encrypted = results[selection-1].cipher;
                        current_plaintext = results[selection-1].plaintext;
                        std::cout << "Applied selection: " << current_plaintext << std::endl;
                    }
                }
                break;
            }
            case 2: {
                // Manual row shift
                std::cout << "Enter row to shift (0-4): ";
                int row;
                std::cin >> row;
                
                if (row >= 0 && row < 5) {
                    std::cout << "Direction (0=left, 1=right): ";
                    bool direction;
                    std::cin >> direction;
                    
                    std::cout << "Number of positions: ";
                    int amount;
                    std::cin >> amount;
                    
                    encrypted.row_shift(row, direction, amount);
                    current_plaintext = ps.translate(encrypted);
                    std::cout << "New plaintext: " << current_plaintext << std::endl;
                } else {
                    std::cout << "Invalid row number." << std::endl;
                }
                break;
            }
            case 3:
                // Show current plaintext
                std::cout << "Current plaintext: " << current_plaintext << std::endl;
                break;
            case 4:
                // Exit
                done = true;
                break;
            default:
                std::cout << "Invalid choice." << std::endl;
        }
    }
}

// Simulated annealing optimizer
decryption_result decrypter::simulated_annealing(const ciphertext& original, 
                                               int iterations,
                                               double initial_temp) const {
    // Create random number generators
    std::mt19937 rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> row_dist(0, 4);             // 5 rows (0-4)
    std::uniform_int_distribution<int> dir_dist(0, 1);             // 0=left, 1=right
    std::uniform_int_distribution<int> amt_dist(1, 4);             // 1-4 positions (increased range)
    std::uniform_real_distribution<double> prob_dist(0.0, 1.0);    // For probability calculations
    
    // Start with the original ciphertext and its score
    ciphertext current = original;
    std::string current_plaintext = ps.translate(current);
    double current_score = score_advanced(current_plaintext);
    
    ciphertext best = current;
    std::string best_plaintext = current_plaintext;
    double best_score = current_score;
    
    std::cout << "Starting simulated annealing with initial score: " 
              << current_score << std::endl;
    
    // Cooling schedule parameters
    double temp = initial_temp;
    const double cooling_rate = 0.97;  // Slower cooling rate for better exploration
    
    // Track plateaus to apply more aggressive moves when stuck
    int plateau_counter = 0;
    const int PLATEAU_THRESHOLD = 100;
    
    // Main annealing loop
    for (int iter = 0; iter < iterations; iter++) {
        // Create a neighbor by applying random row shifts
        ciphertext neighbor = current;
        
        // If we're stuck on a plateau, make more aggressive moves
        bool aggressive_move = (plateau_counter > PLATEAU_THRESHOLD);
        
        // Apply 1-4 random shifts to create a neighbor solution
        int num_shifts = aggressive_move ? 
                        std::uniform_int_distribution<int>(2, 5)(rng) : 
                        std::uniform_int_distribution<int>(1, 3)(rng);
                        
        for (int i = 0; i < num_shifts; i++) {
            int row = row_dist(rng);
            bool direction = dir_dist(rng) == 1;
            int amount = aggressive_move ? 
                        std::uniform_int_distribution<int>(1, 5)(rng) : 
                        amt_dist(rng);
                        
            neighbor.row_shift(row, direction, amount);
        }
        
        // Evaluate the neighbor
        std::string neighbor_plaintext = ps.translate(neighbor);
        double neighbor_score = score_advanced(neighbor_plaintext);
        
        // Compute delta score
        double delta = neighbor_score - current_score;
        
        // Decide whether to accept the neighbor
        bool accept = false;
        if (delta > 0) {
            // Always accept better solutions
            accept = true;
            plateau_counter = 0;  // Reset plateau counter
        } else {
            // Accept worse solutions with probability e^(delta/temp)
            double acceptance_prob = std::exp(delta / temp);
            accept = (prob_dist(rng) < acceptance_prob);
            
            // If no improvement, increment plateau counter
            if (std::abs(delta) < 0.001) {
                plateau_counter++;
            }
        }
        
        // Update current solution if accepted
        if (accept) {
            current = neighbor;
            current_plaintext = neighbor_plaintext;
            current_score = neighbor_score;
            
            // Update best solution if current is better
            if (current_score > best_score) {
                best = current;
                best_plaintext = current_plaintext;
                best_score = current_score;
                
                std::cout << "New best solution found at iteration " << iter 
                          << " with score " << best_score 
                          << ": \"" << best_plaintext.substr(0, 40) 
                          << (best_plaintext.length() > 40 ? "..." : "") << "\"" << std::endl;
            }
        }
        
        // Occasionally try to detect pangrams
        if (iter % 50 == 0 && iter > 0) {
            // Check if current solution has high letter diversity
            std::set<char> unique_letters;
            for (char c : current_plaintext) {
                if (std::isalpha(c)) {
                    unique_letters.insert(std::tolower(c));
                }
            }
            
            // If we have 70% or more of the alphabet, this might be a pangram
            if (unique_letters.size() >= 18) {
                std::cout << "Possible pangram detected - boosting exploration" << std::endl;
                // Temporarily boost temperature to explore more
                temp *= 1.5;
            }
        }
        
        // Cool down
        if (iter % 20 == 0) {
            temp *= cooling_rate;
        }
    }
    
    std::cout << "Simulated annealing completed. Best score: " << best_score << std::endl;
    
    // Return the best solution found
    return decryption_result(best, best_plaintext, best_score);
}

// Extract patterns of repeated words/characters from text
std::vector<std::pair<std::string, size_t>> decrypter::extract_patterns(const std::string& text) const {
    std::vector<std::pair<std::string, size_t>> patterns;
    std::unordered_map<std::string, std::vector<size_t>> word_positions;
    
    // Convert to lowercase and split into words
    std::string lower_text = text;
    std::transform(lower_text.begin(), lower_text.end(), lower_text.begin(), 
                  [](unsigned char c) { return std::tolower(c); });
    
    std::istringstream iss(lower_text);
    std::string word;
    size_t position = 0;
    
    // Track word positions
    while (iss >> word) {
        // Remove punctuation
        word.erase(std::remove_if(word.begin(), word.end(), 
                   [](char c) { return !std::isalpha(c); }), word.end());
        
        if (!word.empty()) {
            word_positions[word].push_back(position);
        }
        position++;
    }
    
    // Extract repeated words and their positions
    for (const auto& wp : word_positions) {
        if (wp.second.size() > 1) {  // Word appears more than once
            patterns.push_back(std::make_pair(wp.first, wp.second[0]));
        }
    }
    
    return patterns;
}

// Check if two pattern sets match
bool decrypter::patterns_match(
    const std::vector<std::pair<std::string, size_t>>& pattern1,
    const std::vector<std::pair<std::string, size_t>>& pattern2) const {
    
    // If sizes are very different, they likely don't match
    if (std::abs(static_cast<int>(pattern1.size() - pattern2.size())) > 2) {
        return false;
    }
    
    // Count matching patterns
    int matches = 0;
    for (const auto& p1 : pattern1) {
        for (const auto& p2 : pattern2) {
            // Check for position matches (allowing for slight shifts)
            if (std::abs(static_cast<int>(p1.second - p2.second)) <= 3) {
                matches++;
                break;
            }
        }
    }
    
    // Require a certain percentage of matches
    double match_ratio = static_cast<double>(matches) / std::min(pattern1.size(), pattern2.size());
    return match_ratio >= 0.5;  // At least 50% of patterns should match
}

// Beam search optimizer
std::vector<decryption_result> decrypter::beam_search(const ciphertext& original, 
                                                     int beam_width,
                                                     int iterations) const {
    // Create random number generators
    std::mt19937 rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    std::uniform_int_distribution<int> row_dist(0, 4);     // 5 rows (0-4)
    std::uniform_int_distribution<int> dir_dist(0, 1);     // 0=left, 1=right
    std::uniform_int_distribution<int> amt_dist(1, 3);     // 1-3 positions
    
    // Initial beam with just the original ciphertext
    std::vector<decryption_result> beam;
    std::string plaintext = ps.translate(original);
    double score = score_advanced(plaintext);
    beam.emplace_back(original, plaintext, score);
    
    std::cout << "Starting beam search with beam width " << beam_width << std::endl;
    
    // Main beam search loop
    for (int iter = 0; iter < iterations; iter++) {
        std::vector<decryption_result> candidates;
        
        // Expand each state in the current beam
        for (const auto& current : beam) {
            // Generate multiple neighbors for each beam state
            for (int i = 0; i < 5; i++) {  // 5 neighbors per beam state
                ciphertext neighbor = current.cipher;
                
                // Apply 1-2 random row shifts to create a neighbor
                int num_shifts = std::uniform_int_distribution<int>(1, 2)(rng);
                for (int j = 0; j < num_shifts; j++) {
                    int row = row_dist(rng);
                    bool direction = dir_dist(rng) == 1;
                    int amount = amt_dist(rng);
                    neighbor.row_shift(row, direction, amount);
                }
                
                // Evaluate the neighbor
                std::string neighbor_plaintext = ps.translate(neighbor);
                double neighbor_score = score_advanced(neighbor_plaintext);
                
                // Add to candidates
                candidates.emplace_back(neighbor, neighbor_plaintext, neighbor_score);
            }
        }
        
        // Combine current beam with new candidates
        beam.insert(beam.end(), candidates.begin(), candidates.end());
        
        // Sort by score (highest first)
        std::sort(beam.begin(), beam.end());
        
        // Keep only the top beam_width candidates
        if (beam.size() > static_cast<size_t>(beam_width)) {
            beam.resize(beam_width);
        }
        
        // Print progress
        if (iter % 10 == 0 || iter == iterations - 1) {
            std::cout << "Iteration " << iter << ", best score: " 
                      << beam[0].score << ", text: \"" 
                      << beam[0].plaintext.substr(0, 40)
                      << (beam[0].plaintext.length() > 40 ? "..." : "") << "\"" << std::endl;
        }
    }
    
    std::cout << "Beam search completed. Found " << beam.size() << " candidates." << std::endl;
    
    return beam;
}

// Auto decrypt - Try multiple strategies until we find something that looks right
decryption_result decrypter::auto_decrypt(const ciphertext& encrypted, 
                                        int max_iterations,
                                        double min_score_threshold) const {
    std::cout << "Starting automatic decryption process..." << std::endl;
    std::cout << "Maximum iterations: " << max_iterations << std::endl;
    std::cout << "Minimum score threshold: " << min_score_threshold << std::endl;
    
    // Start with the original ciphertext
    decryption_result best_result;
    best_result.cipher = encrypted;
    best_result.plaintext = ps.translate(encrypted);
    best_result.score = score_text(best_result.plaintext) * 0.4 + 
                        score_by_dictionary(best_result.plaintext) * 0.6;
    
    std::cout << "Initial plaintext: " << best_result.plaintext << std::endl;
    std::cout << "Initial score: " << std::fixed << std::setprecision(4) << best_result.score << std::endl;
    
    // If the initial text already scores well, we might be done
    if (best_result.score >= min_score_threshold) {
        std::cout << "Initial text already meets threshold criteria." << std::endl;
        return best_result;
    }
    
    // Perform initial analysis
    std::cout << "\nPerforming initial analysis..." << std::endl;
    analyze(encrypted);
    
    // Initialize random number generator for some randomness in our approach
    std::mt19937 rng(static_cast<unsigned int>(std::chrono::system_clock::now().time_since_epoch().count()));
    
    // Try different strategies over multiple iterations
    for (int iter = 0; iter < max_iterations; iter++) {
        std::cout << "\nIteration " << iter + 1 << "/" << max_iterations << std::endl;
        
        // Strategy 1: Row Shifting - Brute force approach
        std::cout << "Trying brute force row shifts..." << std::endl;
        auto row_shift_results = try_all_row_shifts(encrypted, 2);
        
        if (!row_shift_results.empty()) {
            // Get the best result from this strategy
            decryption_result best_shift = row_shift_results[0];
            std::cout << "Best row shift result: \"" << best_shift.plaintext << "\"" << std::endl;
            std::cout << "Score: " << std::fixed << std::setprecision(4) << best_shift.score << std::endl;
            
            // If this is better than our current best, update it
            if (best_shift.score > best_result.score) {
                best_result = best_shift;
                std::cout << "New best result found!" << std::endl;
                
                // If this exceeds our threshold, we can return early
                if (best_result.score >= min_score_threshold) {
                    std::cout << "Score exceeds threshold. Returning result." << std::endl;
                    return best_result;
                }
            }
        }
        
        // Strategy 2: Pattern Matching
        std::cout << "Trying pattern matching..." << std::endl;
        auto pattern_results = try_pattern_matching(encrypted);
        
        if (!pattern_results.empty()) {
            // Get the best result from this strategy
            decryption_result best_pattern = pattern_results[0];
            std::cout << "Best pattern match result: \"" << best_pattern.plaintext << "\"" << std::endl;
            std::cout << "Score: " << std::fixed << std::setprecision(4) << best_pattern.score << std::endl;
            
            // If this is better than our current best, update it
            if (best_pattern.score > best_result.score) {
                best_result = best_pattern;
                std::cout << "New best result found!" << std::endl;
                
                // If this exceeds our threshold, we can return early
                if (best_result.score >= min_score_threshold) {
                    std::cout << "Score exceeds threshold. Returning result." << std::endl;
                    return best_result;
                }
            }
        }
        
        // Strategy 3: Random shifts with hill climbing
        if (iter > 0) { // Skip in first iteration to give deterministic methods a chance
            std::cout << "Trying random shifts with hill climbing..." << std::endl;
            
            // Start with the current best cipher
            ciphertext current = best_result.cipher;
            double current_score = best_result.score;
            
            // Try a series of random shifts and keep if they improve score
            const int random_attempts = 10;
            for (int i = 0; i < random_attempts; i++) {
                // Make a copy to test changes
                ciphertext test = current;
                
                // Apply a random shift
                int row = std::uniform_int_distribution<int>(0, 4)(rng);
                bool direction = std::uniform_int_distribution<int>(0, 1)(rng) == 1;
                int amount = std::uniform_int_distribution<int>(1, 3)(rng);
                
                test.row_shift(row, direction, amount);
                
                // Evaluate the new score
                std::string test_plaintext = ps.translate(test);
                double test_score = score_text(test_plaintext) * 0.4 + 
                                   score_by_dictionary(test_plaintext) * 0.6;
                
                // If better, keep it
                if (test_score > current_score) {
                    current = test;
                    current_score = test_score;
                    std::cout << "Improved with random shift: row " << row 
                              << (direction ? " right" : " left") 
                              << " by " << amount << std::endl;
                    std::cout << "New text: \"" << test_plaintext << "\"" << std::endl;
                    std::cout << "New score: " << std::fixed << std::setprecision(4) 
                              << test_score << std::endl;
                }
            }
            
            // Update best result if improved
            if (current_score > best_result.score) {
                best_result.cipher = current;
                best_result.plaintext = ps.translate(current);
                best_result.score = current_score;
                std::cout << "New best result from random shifts!" << std::endl;
                
                // If this exceeds our threshold, we can return early
                if (best_result.score >= min_score_threshold) {
                    std::cout << "Score exceeds threshold. Returning result." << std::endl;
                    return best_result;
                }
            }
        }
    }
    
    std::cout << "\nAutomatic decryption completed." << std::endl;
    std::cout << "Best result: \"" << best_result.plaintext << "\"" << std::endl;
    std::cout << "Final score: " << std::fixed << std::setprecision(4) << best_result.score << std::endl;
    
    return best_result;
}

// Improved auto decrypt using advanced techniques
decryption_result decrypter::advanced_decrypt(const ciphertext& encrypted,
                                           int max_iterations,
                                           double min_score_threshold) const {
    std::cout << "Starting advanced decryption process..." << std::endl;
    std::cout << "Maximum iterations: " << max_iterations << std::endl;
    std::cout << "Minimum score threshold: " << min_score_threshold << std::endl;
    
    // Track the best result
    decryption_result best_result;
    best_result.cipher = encrypted;
    best_result.plaintext = ps.translate(encrypted);
    best_result.score = score_advanced(best_result.plaintext);
    
    std::cout << "Initial plaintext: " << best_result.plaintext << std::endl;
    std::cout << "Initial score: " << std::fixed << std::setprecision(4) << best_result.score << std::endl;
    
    // If the initial text already scores well, we might be done
    if (best_result.score >= min_score_threshold) {
        std::cout << "Initial text already meets threshold criteria." << std::endl;
        return best_result;
    }
    
    // Check for pangram-like patterns
    std::set<char> unique_letters;
    for (char c : best_result.plaintext) {
        if (std::isalpha(c)) {
            unique_letters.insert(std::tolower(c));
        }
    }
    
    bool likely_pangram = unique_letters.size() >= 18;  // 70% of alphabet
    if (likely_pangram) {
        std::cout << "Text appears to have high letter diversity - might be a pangram" << std::endl;
        std::cout << "Adjusting optimization strategy accordingly" << std::endl;
    }
    
    // Perform initial analysis for diagnostic information
    std::cout << "\nPerforming initial analysis..." << std::endl;
    analyze(encrypted);
    
    // Strategy 1: Beam Search with adjusted parameters for pangrams
    std::cout << "\nRunning beam search optimization..." << std::endl;
    int beam_width = likely_pangram ? 12 : 8;  // Wider beam for pangrams
    const int beam_iterations = std::max(50, max_iterations * (likely_pangram ? 10 : 8));
    
    auto beam_results = beam_search(encrypted, beam_width, beam_iterations);
    if (!beam_results.empty()) {
        decryption_result best_beam = beam_results[0];
        std::cout << "Best beam search result: \"" << best_beam.plaintext << "\"" << std::endl;
        std::cout << "Score: " << std::fixed << std::setprecision(4) << best_beam.score << std::endl;
        
        if (best_beam.score > best_result.score) {
            best_result = best_beam;
            std::cout << "New best result found!" << std::endl;
            
            if (best_result.score >= min_score_threshold) {
                std::cout << "Score exceeds threshold. Returning result." << std::endl;
                return best_result;
            }
        }
    }
    
    // Strategy 2: Simulated Annealing with adjusted parameters for pangrams
    std::cout << "\nRunning simulated annealing optimization..." << std::endl;
    const int sa_iterations = likely_pangram ? 
                             std::max(500, max_iterations * 25) : 
                             std::max(250, max_iterations * 20);
    const double initial_temp = likely_pangram ? 20.0 : 15.0;
    
    decryption_result sa_result = simulated_annealing(encrypted, sa_iterations, initial_temp);
    std::cout << "Best simulated annealing result: \"" << sa_result.plaintext << "\"" << std::endl;
    std::cout << "Score: " << std::fixed << std::setprecision(4) << sa_result.score << std::endl;
    
    if (sa_result.score > best_result.score) {
        best_result = sa_result;
        std::cout << "New best result found!" << std::endl;
        
        if (best_result.score >= min_score_threshold) {
            std::cout << "Score exceeds threshold. Returning result." << std::endl;
            return best_result;
        }
    }
    
    // Strategy 3: Try a hybrid approach starting from best results so far
    std::cout << "\nRunning hybrid optimization approach..." << std::endl;
    
    // Start with our current best and apply brute force row shifts
    auto row_shift_results = try_all_row_shifts(best_result.cipher, likely_pangram ? 4 : 3);
    if (!row_shift_results.empty()) {
        decryption_result best_shift = row_shift_results[0];
        std::cout << "Best hybrid row shift result: \"" << best_shift.plaintext << "\"" << std::endl;
        std::cout << "Score: " << std::fixed << std::setprecision(4) << best_shift.score << std::endl;
        
        if (best_shift.score > best_result.score) {
            best_result = best_shift;
            std::cout << "New best result found!" << std::endl;
            
            if (best_result.score >= min_score_threshold) {
                std::cout << "Score exceeds threshold. Returning result." << std::endl;
                return best_result;
            }
        }
    }
    
    // Extract pattern information from the current best plaintext
    auto patterns = extract_patterns(best_result.plaintext);
    
    // Strategy 4: Check against common phrases or patterns with special pangram phrases
    std::cout << "\nChecking for common patterns..." << std::endl;
    std::vector<std::string> common_phrases = {
        "the", "and", "that", "have", "for", "with", "this", "from", "they", "would",
        "there", "their", "what", "about", "when", "your", "know", "will"
    };
    
    // Add specific pangram patterns if text appears to be a pangram
    if (likely_pangram) {
        std::vector<std::string> pangram_phrases = {
            "quick", "sphinx", "black", "quartz", "judge", "vow", "pack", "box", 
            "five", "dozen", "liquor", "jugs", "jump", "vexing", "daft", "zebra",
            "waltz", "nymph", "jigs", "vex", "quip"
        };
        common_phrases.insert(common_phrases.end(), pangram_phrases.begin(), pangram_phrases.end());
    }
    
    // Try to match each common phrase at different positions
    for (const auto& phrase : common_phrases) {
        if (phrase.length() >= 3) {  // Include shorter phrases for pangrams
            for (size_t pos = 0; pos < best_result.plaintext.length() - phrase.length(); pos++) {
                // Make a copy of the current best cipher
                ciphertext test_cipher = best_result.cipher;
                
                // Try different combinations of row shifts to make this phrase appear
                for (int shifts = 0; shifts < 15; shifts++) {  // More shift combinations
                    // Apply a combination of shifts
                    test_cipher.row_shift(shifts % 5, shifts % 2 == 0, 1 + (shifts % 4));
                    
                    // Translate and check
                    std::string test_plaintext = ps.translate(test_cipher);
                    
                    // Check if our phrase appears at the expected position
                    bool found = true;
                    for (size_t i = 0; i < phrase.length(); i++) {
                        if (pos + i >= test_plaintext.length() || 
                            std::tolower(test_plaintext[pos + i]) != phrase[i]) {
                            found = false;
                            break;
                        }
                    }
                    
                    if (found) {
                        // Score the result
                        double test_score = score_advanced(test_plaintext);
                        
                        // If better, update best result
                        if (test_score > best_result.score) {
                            best_result.cipher = test_cipher;
                            best_result.plaintext = test_plaintext;
                            best_result.score = test_score;
                            
                            std::cout << "Found phrase \"" << phrase << "\" at position " << pos << std::endl;
                            std::cout << "New best text: \"" << test_plaintext << "\"" << std::endl;
                            std::cout << "New best score: " << test_score << std::endl;
                            
                            if (best_result.score >= min_score_threshold) {
                                std::cout << "Score exceeds threshold. Returning result." << std::endl;
                                return best_result;
                            }
                        }
                    }
                }
            }
        }
    }
    
    std::cout << "\nAdvanced decryption completed." << std::endl;
    std::cout << "Best result: \"" << best_result.plaintext << "\"" << std::endl;
    std::cout << "Final score: " << std::fixed << std::setprecision(4) << best_result.score << std::endl;
    
    return best_result;
} 