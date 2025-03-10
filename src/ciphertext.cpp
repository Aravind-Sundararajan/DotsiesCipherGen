#include "ciphertext.h"
#include <iostream>
#include <algorithm>

// Constructors
ciphertext::ciphertext() : phrase(1) {}

ciphertext::ciphertext(std::size_t s) : phrase(s) {}

ciphertext::ciphertext(const ciphertext& other) : phrase(other.phrase) {}

ciphertext& ciphertext::operator=(const ciphertext& other) {
    if (this != &other) {
        phrase = other.phrase;
    }
    return *this;
}

// Core operations
letter& ciphertext::operator()(std::size_t x) {
    return phrase[x];
}

const letter& ciphertext::operator()(std::size_t x) const {
    return phrase[x];
}

letter& ciphertext::get(std::size_t x) {
    return phrase[x];
}

const letter& ciphertext::get(std::size_t x) const {
    return phrase[x];
}

void ciphertext::set(std::size_t x, const letter& l) {
    phrase[x] = l;
}

ciphertext ciphertext::append(const letter& l) {
    ciphertext result(size() + 1);
    result.set_all(*this);
    result.set(size(), l);
    return result;
}

void ciphertext::set_all(const ciphertext& c) {
    std::size_t copy_size = std::min(size(), c.size());
    for (std::size_t x = 0; x < copy_size; x++) {
        set(x, c.get(x));
    }
}

// Transformations
void ciphertext::row_swap(size_t p1, size_t p2) {
    for (auto& letter : phrase) {
        letter.swap(p1, p2);
    }
}

void ciphertext::column_swap(size_t p1, size_t p2) {
    if (p1 >= size() || p2 >= size()) return;
    std::swap(phrase[p1], phrase[p2]);
}

void ciphertext::column_shift() {
    for (auto& letter : phrase) {
        letter.circle_shift();
    }
}

void ciphertext::column_shift(bool dir) {
    for (auto& letter : phrase) {
        letter.circle_shift(dir);
    }
}

void ciphertext::column_shift(bool dir, int amount) {
    for (auto& letter : phrase) {
        letter.circle_shift(dir, amount);
    }
}

void ciphertext::row_shift(std::size_t r) {
    if (empty() || r >= 5) return;
    
    bool temp = phrase[0].b[r];
    for (std::size_t x = 0; x < size() - 1; x++) {
        phrase[x].b.set(r, phrase[x + 1].b[r]);
    }
    phrase.back().b.set(r, temp);
}

void ciphertext::row_shift(std::size_t r, bool direction) {
    if (empty() || r >= 5) return;

    if (direction) {
        // Shift up
        bool temp = phrase[0].b[r];
        for (std::size_t x = 0; x < size() - 1; x++) {
            phrase[x].b.set(r, phrase[x + 1].b[r]);
        }
        phrase.back().b.set(r, temp);
    } else {
        // Shift down
        bool temp = phrase.back().b[r];
        for (std::size_t x = size() - 1; x > 0; x--) {
            phrase[x].b.set(r, phrase[x - 1].b[r]);
        }
        phrase[0].b.set(r, temp);
    }
}

// Utility
void ciphertext::print() const {
    for (const auto& letter : phrase) {
        letter.printb();
    }
}
