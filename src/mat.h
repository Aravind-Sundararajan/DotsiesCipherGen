#ifndef MAT_H_
#define MAT_H_
#include "base.h"
#include <vector>
#include <algorithm>
#include <stdexcept>
#include <type_traits>

class mat {
public:
	// Constructors and destructor
	mat() : rows(0), cols(0) {}
	mat(size_t r, size_t c) : rows(r), cols(c), data(r * c, false) {}
	~mat() = default;
	
	// Copy operations
	mat(const mat& other) = default;
	mat& operator=(const mat& other) = default;
	
	// Move operations
	mat(mat&& other) noexcept = default;
	mat& operator=(mat&& other) noexcept = default;

	// Fast accessors for internal use (no bounds checking)
	inline bool get_unchecked(size_t row, size_t col) const {
		return data[row * cols + col];
	}
	
	inline void set_unchecked(size_t row, size_t col, bool value) {
		data[row * cols + col] = value;
	}

	// Getters and setters with bounds checking
	bool get(size_t row, size_t col) const {
		#ifdef NDEBUG
			return get_unchecked(row, col);
		#else
			validate_indices(row, col);
			return get_unchecked(row, col);
		#endif
	}
	
	void set(size_t row, size_t col, bool value) {
		#ifdef NDEBUG
			set_unchecked(row, col, value);
		#else
			validate_indices(row, col);
			set_unchecked(row, col, value);
		#endif
	}
	
	bool operator()(size_t row, size_t col) const {
		return get(row, col);
	}
	
	// Reshaping operations
	void reshape(size_t new_rows, size_t new_cols) {
		if (new_rows * new_cols != rows * cols) {
			// Need to resize the underlying vector
			data.resize(new_rows * new_cols, false);
		}
		rows = new_rows;
		cols = new_cols;
	}
	
	void resize(size_t new_rows, size_t new_cols) {
		if (new_rows == rows && new_cols == cols) return;
		
		std::vector<bool> new_data(new_rows * new_cols, false);
		
		// Copy existing data where it fits
		const size_t min_rows = std::min(rows, new_rows);
		const size_t min_cols = std::min(cols, new_cols);
		
		// Fast path for contiguous memory if possible (common column count)
		if (cols == new_cols) {
			// We can copy rows in chunks
			for (size_t r = 0; r < min_rows; ++r) {
				const size_t old_offset = r * cols;
				const size_t new_offset = r * new_cols;
				for (size_t c = 0; c < min_cols; ++c) {
					new_data[new_offset + c] = data[old_offset + c];
				}
			}
		} else {
			// Have to copy cell by cell
			for (size_t r = 0; r < min_rows; ++r) {
				for (size_t c = 0; c < min_cols; ++c) {
					new_data[r * new_cols + c] = get_unchecked(r, c);
				}
			}
		}
		
		data = std::move(new_data);
		rows = new_rows;
		cols = new_cols;
	}
	
	// Row operations
	void row_shift(size_t row, bool direction = true) {
		if (row >= rows || cols <= 1) return;
		
		if (direction) { // shift right
			bool last = get_unchecked(row, cols - 1);
			for (size_t c = cols - 1; c > 0; --c) {
				set_unchecked(row, c, get_unchecked(row, c - 1));
			}
			set_unchecked(row, 0, last);
		} else { // shift left
			bool first = get_unchecked(row, 0);
			for (size_t c = 0; c < cols - 1; ++c) {
				set_unchecked(row, c, get_unchecked(row, c + 1));
			}
			set_unchecked(row, cols - 1, first);
		}
	}
	
	void row_shift(size_t row, bool direction, size_t amount);
	
	void row_swap(size_t row1, size_t row2) {
		if (row1 >= rows || row2 >= rows || row1 == row2) return;
		
		// Fast path: swap entire rows using vector swapping
		// Instead of bit-by-bit swap
		for (size_t c = 0; c < cols; ++c) {
			bool temp = get_unchecked(row1, c);
			set_unchecked(row1, c, get_unchecked(row2, c));
			set_unchecked(row2, c, temp);
		}
	}
	
	// Column operations
	void col_shift(size_t col, bool direction = true) {
		if (col >= cols || rows <= 1) return;
		
		if (direction) { // shift down
			bool last = get_unchecked(rows - 1, col);
			for (size_t r = rows - 1; r > 0; --r) {
				set_unchecked(r, col, get_unchecked(r - 1, col));
			}
			set_unchecked(0, col, last);
		} else { // shift up
			bool first = get_unchecked(0, col);
			for (size_t r = 0; r < rows - 1; ++r) {
				set_unchecked(r, col, get_unchecked(r + 1, col));
			}
			set_unchecked(rows - 1, col, first);
		}
	}
	
	void col_shift(size_t col, bool direction, size_t amount);
	
	void col_swap(size_t col1, size_t col2) {
		if (col1 >= cols || col2 >= cols || col1 == col2) return;
		
		for (size_t r = 0; r < rows; ++r) {
			bool temp = get_unchecked(r, col1);
			set_unchecked(r, col1, get_unchecked(r, col2));
			set_unchecked(r, col2, temp);
		}
	}
	
	// Load/set from bitsets (optimized template version)
	template<size_t N>
	void set_row(size_t row, const std::bitset<N>& bits) {
		if (row >= rows) return;
		const size_t use_bits = std::min(cols, N);
		
		for (size_t c = 0; c < use_bits; ++c) {
			set_unchecked(row, c, bits[c]);
		}
	}
	
	template<size_t N>
	std::bitset<N> get_row_as_bitset(size_t row) const {
		std::bitset<N> result;
		if (row >= rows) return result;
		
		const size_t use_bits = std::min(cols, N);
		for (size_t c = 0; c < use_bits; ++c) {
			result[c] = get_unchecked(row, c);
		}
		return result;
	}
	
	// Additional operations implemented in cpp file
	void transpose();
	mat submatrix(size_t start_row, size_t start_col, size_t num_rows, size_t num_cols) const;
	size_t count() const;
	void set_all(bool value);
	
	// Utility methods
	size_t num_rows() const noexcept { return rows; }
	size_t num_cols() const noexcept { return cols; }
	bool empty() const noexcept { return rows == 0 || cols == 0; }
	
	void clear() { 
		std::fill(data.begin(), data.end(), false); 
	}
	
	// Flip all bits in a row (optimized)
	void flip_row(size_t row) {
		if (row >= rows) return;
		for (size_t c = 0; c < cols; ++c) {
			const size_t idx = row * cols + c;
			data[idx] = !data[idx];
		}
	}
	
	// Flip all bits in a column (optimized)
	void flip_col(size_t col) {
		if (col >= cols) return;
		for (size_t r = 0; r < rows; ++r) {
			const size_t idx = r * cols + col;
			data[idx] = !data[idx];
		}
	}
	
	// Flip all bits in the matrix
	void flip_all() {
		for (size_t i = 0; i < data.size(); ++i) {
			data[i] = !data[i];
		}
	}
	
	// Print matrix for debugging
	void print() const {
		for (size_t r = 0; r < rows; ++r) {
			for (size_t c = 0; c < cols; ++c) {
				std::cout << (get_unchecked(r, c) ? "#" : " ");
			}
			std::cout << std::endl;
		}
	}

private:
	size_t rows = 0;
	size_t cols = 0;
	std::vector<bool> data;
	
	// Helper methods
	size_t index(size_t row, size_t col) const noexcept {
		return row * cols + col;
	}
	
	void validate_indices(size_t row, size_t col) const {
		if (row >= rows || col >= cols) {
			throw std::out_of_range("Matrix indices out of range");
		}
	}
};

#endif
