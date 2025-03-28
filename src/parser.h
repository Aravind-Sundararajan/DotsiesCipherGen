#ifndef PARSER_H_
#define PARSER_H_
#include "letter.h"
#include "kvp.h"
#include "table.h"
#include "ciphertext.h"
#include "mat.h"
#include <string>
#include <vector>
#include <memory>

class parser {
public:
	// Constructor that takes a reference to the table
	explicit parser(const table& t) noexcept : tab(&t) {}
	
	// Destructor - table is not owned by parser, so no need to delete
	~parser() = default;
	
	// Copy constructor
	parser(const parser& p) noexcept = default;
	
	// Move constructor
	parser(parser&& p) noexcept = default;
	
	// Copy assignment
	parser& operator=(const parser& p) noexcept = default;
	
	// Move assignment
	parser& operator=(parser&& p) noexcept = default;
	
	// Translate a C-style string to ciphertext
	ciphertext translate(const char* plaintext) const;
	
	// Translate a std::string to ciphertext
	ciphertext translate(const std::string& plaintext) const;
	
	// Translate ciphertext to a C-style character array
	void translate(const ciphertext& c, char* oStr) const;
	
	// Translate ciphertext to std::string (more modern API)
	std::string translate(const ciphertext& c) const;
	
	// Optional batch processing methods for large texts
	// Process multiple characters at once for better cache locality
	ciphertext translate_batch(const char* plaintext, size_t length, size_t batch_size = 64) const;
	
	// Cache-optimized translations with pre-reservation
	// This can be significantly faster for large text volumes
	ciphertext translate_optimized(const std::string& plaintext) const;

private:
	// Pointer to the table (not owned by parser)
	const table* tab;
};

#endif // PARSER_H_
