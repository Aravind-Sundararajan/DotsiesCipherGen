#ifndef CIP_H_
#define CIP_H_

#include "base.h"
#include "letter.h"
#include <vector>
#include <memory>

class ciphertext {
public:
    // Constructors and destructor
    ciphertext();
    explicit ciphertext(std::size_t s);
    ~ciphertext() = default;
    ciphertext(const ciphertext& other);
    ciphertext& operator=(const ciphertext& other);
    ciphertext(ciphertext&& other) noexcept = default;
    ciphertext& operator=(ciphertext&& other) noexcept = default;

    // Core operations
    letter& operator()(std::size_t x);
    const letter& operator()(std::size_t x) const;
    letter& get(std::size_t x);
    const letter& get(std::size_t x) const;
    void set(std::size_t x, const letter& l);
    ciphertext append(const letter& l);
    void set_all(const ciphertext& c);

    // Transformations
    void row_swap(size_t p1, size_t p2);
    void column_swap(size_t p1, size_t p2);
    void column_shift();
    void column_shift(bool dir);
    void column_shift(bool dir, int amount);
    void row_shift(std::size_t r);
    void row_shift(std::size_t r, bool direction);

    // Utility
    void print() const;
    std::size_t size() const { return phrase.size(); }
    bool empty() const { return phrase.empty(); }

    // Add bit access method
    bool getBit(std::size_t letterIndex, std::size_t bitIndex) const {
        return phrase[letterIndex].b[bitIndex];
    }

private:
    std::vector<letter> phrase;
};

#endif
