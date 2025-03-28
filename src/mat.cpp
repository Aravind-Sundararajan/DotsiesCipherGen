#include "mat.h"
#include <iostream>
#include <numeric>

// Note: Most methods are implemented inline in the header.
// This file contains additional implementations that might be added in the future.

// Optimized version of row_shift that can handle multiple shifts at once more efficiently
// by calculating the exact positions rather than doing multiple individual shifts
void mat::row_shift(size_t row, bool direction, size_t amount) {
    if (row >= rows || cols <= 1 || amount == 0) return;
    
    // Calculate effective shift amount (take modulo to avoid unnecessary rotations)
    amount = amount % cols;
    if (amount == 0) return;
    
    // Allocate buffer on stack for small matrices, heap for large ones
    constexpr size_t STACK_BUFFER_SIZE = 256;
    bool stack_buffer[STACK_BUFFER_SIZE];
    std::vector<bool> heap_buffer;
    
    // Create a temporary copy of the row
    const size_t row_offset = row * cols;
    
    if (cols <= STACK_BUFFER_SIZE) {
        // Use stack buffer
        for (size_t c = 0; c < cols; ++c) {
            stack_buffer[c] = data[row_offset + c];
        }
        
        // Apply the shift using stack buffer
        if (direction) { // right shift
            for (size_t c = 0; c < cols; ++c) {
                data[row_offset + ((c + amount) % cols)] = stack_buffer[c];
            }
        } else { // left shift
            for (size_t c = 0; c < cols; ++c) {
                data[row_offset + ((c + cols - amount) % cols)] = stack_buffer[c];
            }
        }
    } else {
        // Use heap buffer
        heap_buffer.resize(cols);
        for (size_t c = 0; c < cols; ++c) {
            heap_buffer[c] = data[row_offset + c];
        }
        
        // Apply the shift using heap buffer
        if (direction) { // right shift
            for (size_t c = 0; c < cols; ++c) {
                data[row_offset + ((c + amount) % cols)] = heap_buffer[c];
            }
        } else { // left shift
            for (size_t c = 0; c < cols; ++c) {
                data[row_offset + ((c + cols - amount) % cols)] = heap_buffer[c];
            }
        }
    }
}

// Optimized version of col_shift that can handle multiple shifts at once more efficiently
void mat::col_shift(size_t col, bool direction, size_t amount) {
    if (col >= cols || rows <= 1 || amount == 0) return;
    
    // Calculate effective shift amount (take modulo to avoid unnecessary rotations)
    amount = amount % rows;
    if (amount == 0) return;
    
    // Allocate buffer on stack for small matrices, heap for large ones
    constexpr size_t STACK_BUFFER_SIZE = 256;
    bool stack_buffer[STACK_BUFFER_SIZE];
    std::vector<bool> heap_buffer;
    
    if (rows <= STACK_BUFFER_SIZE) {
        // Use stack buffer
        // Create a temporary copy of the column
        for (size_t r = 0; r < rows; ++r) {
            stack_buffer[r] = data[r * cols + col];
        }
        
        // Apply the shift using stack buffer
        if (direction) { // down shift
            for (size_t r = 0; r < rows; ++r) {
                data[((r + amount) % rows) * cols + col] = stack_buffer[r];
            }
        } else { // up shift
            for (size_t r = 0; r < rows; ++r) {
                data[((r + rows - amount) % rows) * cols + col] = stack_buffer[r];
            }
        }
    } else {
        // Use heap buffer
        heap_buffer.resize(rows);
        // Create a temporary copy of the column
        for (size_t r = 0; r < rows; ++r) {
            heap_buffer[r] = data[r * cols + col];
        }
        
        // Apply the shift using heap buffer
        if (direction) { // down shift
            for (size_t r = 0; r < rows; ++r) {
                data[((r + amount) % rows) * cols + col] = heap_buffer[r];
            }
        } else { // up shift
            for (size_t r = 0; r < rows; ++r) {
                data[((r + rows - amount) % rows) * cols + col] = heap_buffer[r];
            }
        }
    }
}

// Transpose the matrix (rows become columns and vice versa)
void mat::transpose() {
    if (empty()) return;
    
    if (rows == cols) {
        // For square matrices, we can do in-place transposition
        for (size_t r = 0; r < rows; ++r) {
            for (size_t c = r + 1; c < cols; ++c) {
                const size_t idx1 = r * cols + c;
                const size_t idx2 = c * cols + r;
                std::swap(data[idx1], data[idx2]);
            }
        }
    } else {
        // For non-square matrices, create a new data structure
        const size_t new_rows = cols;
        const size_t new_cols = rows;
        std::vector<bool> new_data(new_rows * new_cols);
        
        for (size_t r = 0; r < rows; ++r) {
            const size_t row_offset = r * cols;
            for (size_t c = 0; c < cols; ++c) {
                // In transposed matrix, (r,c) becomes (c,r)
                new_data[c * new_cols + r] = data[row_offset + c];
            }
        }
        
        data = std::move(new_data);
        std::swap(rows, cols);
    }
}

// Create a submatrix (view) of the original matrix
mat mat::submatrix(size_t start_row, size_t start_col, size_t num_rows, size_t num_cols) const {
    // Check bounds
    if (start_row + num_rows > rows || start_col + num_cols > cols) {
        throw std::out_of_range("Submatrix exceeds matrix bounds");
    }
    
    mat result(num_rows, num_cols);
    for (size_t r = 0; r < num_rows; ++r) {
        const size_t src_row_offset = (start_row + r) * cols;
        const size_t dst_row_offset = r * num_cols;
        
        for (size_t c = 0; c < num_cols; ++c) {
            result.data[dst_row_offset + c] = data[src_row_offset + start_col + c];
        }
    }
    
    return result;
}

// Count number of set bits in the matrix
size_t mat::count() const {
    // Use std::count for better potential vectorization
    return std::count(data.begin(), data.end(), true);
}

// Set all bits in the matrix
void mat::set_all(bool value) {
    std::fill(data.begin(), data.end(), value);
}
