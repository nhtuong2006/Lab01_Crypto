#ifndef PRIMALITY_H
#define PRIMALITY_H

// ============================================================
//  PHẦN CỦA BẠN B: số ngẫu nhiên lớn + kiểm tra nguyên tố
// ============================================================

#include <vector>
#include "bigint.h"
#include "prng.h"

// Đã cài sẵn: danh sách số nguyên tố nhỏ < limit (sàng Eratosthenes)
std::vector<uint32_t> small_primes(uint32_t limit);

// TODO(B):
// n mod d với d nhỏ (32 bit), dùng cho chia thử nhanh
uint32_t mod_small(const uint512_t &n, uint32_t d);

// Sinh số ngẫu nhiên đúng `bits` bit: bit cao nhất = 1 (bit thứ bits-1).
// Nếu make_odd = true thì bit thấp nhất = 1.
uint512_t random_bits(prng_ctx *ctx, size_t bits, bool make_odd = false);

// Sinh số ngẫu nhiên đều trong [lo, hi] (khuyến nghị rejection sampling, không dùng % )
uint512_t random_range(prng_ctx *ctx, const uint512_t &lo, const uint512_t &hi);

// Miller-Rabin với `rounds` cơ sở ngẫu nhiên. Trả true nếu "có thể nguyên tố".
// Cần xử lý riêng: n < 2, n chẵn, n là số nguyên tố nhỏ.
bool is_probable_prime(prng_ctx *ctx, const uint512_t &n, int rounds = 20);

#endif
