#ifndef DH_H
#define DH_H

#include <stdint.h>
#include "prng.h"
#include "bigint.h"

// TODO(A): result = base^exp mod mod  (square-and-multiply, dùng mulmod)
uint512_t modular_exponentiation(const uint512_t &base, const uint512_t &exp, const uint512_t &mod);

// TODO(B): p có đúng bitsize bit, p và (p-1)/2 đều nguyên tố.
uint512_t generate_safe_prime(prng_ctx *ctx, size_t bitsize = 512);

// TODO(B): khóa riêng ngẫu nhiên trong [2, p-2]
uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p);

// TODO(A hoặc B, chia nhau): chọn phần tử sinh g cho safe prime p.
// Với p = 2q+1: g hợp lệ nếu g^2 != 1 và g^q != 1 (mod p). Thử g = 2, 3, 5, ...
uint512_t find_generator(const uint512_t &p);

#endif
