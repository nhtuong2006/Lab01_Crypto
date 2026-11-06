#ifndef BIGINT_H
#define BIGINT_H

// ============================================================
//  PHẦN CỦA BẠN A: số nguyên lớn không dấu (little-endian limbs)
//  digits[0] là 64 bit thấp nhất.
// ============================================================

#include <stdint.h>
#include <stddef.h>
#include <string>

// Kiểu 128 bit của GCC/Clang; __extension__ để không bị cảnh báo -Wpedantic
__extension__ typedef unsigned __int128 uint128_t;

struct uint512_t
{
    uint64_t digits[8];
    uint512_t();
    explicit uint512_t(uint64_t v); // gán số nhỏ vào digits[0]
};

// ---------- Đã cài sẵn (tiện ích, không cần sửa) ----------
bool is_zero(const uint512_t &a);
size_t bit_length(const uint512_t &a);            // số bit có nghĩa, 0 -> 0
bool get_bit(const uint512_t &a, size_t i);       // bit thứ i (0 = thấp nhất)
void set_bit(uint512_t &a, size_t i);
std::string to_hex(const uint512_t &a);           // không có "0x", chữ thường
bool from_hex(const std::string &s, uint512_t &out);

// ---------- TODO(A): cài trong bigint.cpp ----------
// So sánh: trả về -1 nếu a<b, 0 nếu a==b, +1 nếu a>b
int cmp(const uint512_t &a, const uint512_t &b);

uint512_t operator+(const uint512_t &a, const uint512_t &b); // wrap-around mod 2^512
uint512_t operator-(const uint512_t &a, const uint512_t &b); // giả sử a >= b

uint512_t shl1(const uint512_t &a);               // a << 1
uint512_t shr1(const uint512_t &a);               // a >> 1

// Rút gọn: x mod m (m != 0). Cho phép x >= m bất kỳ.
uint512_t mod_reduce(const uint512_t &x, const uint512_t &m);
// (x + y) mod m, giả sử x, y < m. KHÔNG được để tràn 512 bit (xem AddMod của thầy).
uint512_t addmod(const uint512_t &x, const uint512_t &y, const uint512_t &m);
// (x * y) mod m theo kiểu double-and-add trên từng bit của y (MultMod của thầy).
uint512_t mulmod(const uint512_t &x, const uint512_t &y, const uint512_t &m);

// ---------- Toán tử so sánh (dựa trên cmp, đã cài sẵn) ----------
bool operator==(const uint512_t &a, const uint512_t &b);
bool operator!=(const uint512_t &a, const uint512_t &b);
bool operator<(const uint512_t &a, const uint512_t &b);
bool operator>(const uint512_t &a, const uint512_t &b);
bool operator<=(const uint512_t &a, const uint512_t &b);
bool operator>=(const uint512_t &a, const uint512_t &b);

#endif
