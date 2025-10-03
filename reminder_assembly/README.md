This project provides an implementation of a multiword integer division routine in assembly language. The function performs signed integer division with remainder on arbitrarily large integers represented as arrays of 64-bit words.

Both dividend, divisor, quotient, and remainder are treated as two’s complement signed integers.

Function: int64_t mdiv(int64_t *x, size_t n, int64_t y).

x — pointer to an array of n 64-bit integers (int64_t) representing the dividend. The number has 64 * n bits. Stored in memory in little-endian order (least significant word at the lowest address). n — number of elements in array x (size of the dividend in 64-bit words). y — 64-bit divisor (int64_t).

Function returns the remainder of the division (int64_t).
