#include "mat.h"
#include <iostream>
#include <bitset>

int main() {
    // Create a 5x5 matrix
    mat m(5, 5);
    
    // Initialize with a pattern
    for (size_t r = 0; r < m.num_rows(); ++r) {
        for (size_t c = 0; c < m.num_cols(); ++c) {
            // Create a diagonal pattern
            m.set(r, c, r == c);
        }
    }
    
    std::cout << "Original 5x5 matrix with diagonal pattern:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Demonstrate row operations
    m.row_shift(1, true);  // Shift row 1 right
    std::cout << "After shifting row 1 right:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    m.row_shift(2, false, 2);  // Shift row 2 left by 2 positions
    std::cout << "After shifting row 2 left by 2 positions:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Demonstrate column operations
    m.col_shift(3, true);  // Shift column 3 down
    std::cout << "After shifting column 3 down:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Demonstrate row/column swaps
    m.row_swap(0, 4);  // Swap rows 0 and 4
    std::cout << "After swapping rows 0 and 4:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Demonstrate bitset operations
    std::bitset<5> bits("10101");
    m.set_row(3, bits);
    std::cout << "After setting row 3 from bitset 10101:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Retrieve a row as bitset
    std::bitset<5> row_bits = m.get_row_as_bitset<5>(3);
    std::cout << "Row 3 as bitset: " << row_bits << std::endl << std::endl;
    
    // Transpose the matrix
    m.transpose();
    std::cout << "After transposing the matrix:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Reshape to 10x2 (note this keeps the data linearized)
    m.reshape(10, 2);
    std::cout << "After reshaping to 10x2:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Create a submatrix
    mat sub = m.submatrix(0, 0, 3, 2);
    std::cout << "Submatrix (3x2 from top-left):" << std::endl;
    sub.print();
    std::cout << std::endl;
    
    // Resize to smaller dimensions
    m.resize(4, 4);
    std::cout << "After resizing to 4x4:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Flip a row
    m.flip_row(1);
    std::cout << "After flipping row 1:" << std::endl;
    m.print();
    std::cout << std::endl;
    
    // Count set bits
    std::cout << "Number of set bits in matrix: " << m.count() << std::endl;
    
    return 0;
} 