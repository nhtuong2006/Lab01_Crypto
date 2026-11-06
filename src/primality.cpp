#include "primality.h"
#include "dh.h" // modular_exponentiation dùng trong Miller-Rabin

std::vector<uint32_t> small_primes(uint32_t limit)
{
    std::vector<bool> comp(limit + 1, false);
    std::vector<uint32_t> out;
    for (uint32_t i = 2; i < limit; i++)
    {
        if (comp[i])
            continue;
        out.push_back(i);
        for (uint64_t j = (uint64_t)i * i; j < limit; j += i)
            comp[(size_t)j] = true;
    }
    return out;
}

// ============================================================
//  TODO(B)
// ============================================================

uint32_t mod_small(const uint512_t &n, uint32_t d)
{
    // TODO(B): duyệt limb từ cao xuống thấp với uint128_t:
    //   r = ((r << 64) | limb) % d
    (void)n;
    (void)d;
    return 1;
}

uint512_t random_bits(prng_ctx *ctx, size_t bits, bool make_odd)
{
    // TODO(B): lấy đủ limb từ prng_next_u64(ctx), che bớt bit thừa,
    //          đặt bit cao nhất (bits-1) = 1, nếu make_odd thì bit 0 = 1.
    (void)ctx;
    (void)bits;
    (void)make_odd;
    return uint512_t();
}

uint512_t random_range(prng_ctx *ctx, const uint512_t &lo, const uint512_t &hi)
{
    // TODO(B): rejection sampling. Gợi ý: span = hi - lo + 1; sinh số
    //          bit_length(span) bit (KHÔNG ép bit cao), lặp tới khi < span, rồi + lo.
    (void)ctx;
    (void)lo;
    (void)hi;
    return uint512_t();
}

bool is_probable_prime(prng_ctx *ctx, const uint512_t &n, int rounds)
{
    // TODO(B): Miller-Rabin
    //  1. Loại n < 2, n chẵn (trừ n == 2); chia thử cho các số nguyên tố nhỏ.
    //  2. n-1 = 2^s * d, d lẻ.
    //  3. Lặp `rounds` lần: a = random_range(ctx, 2, n-2);
    //     x = modular_exponentiation(a, d, n);
    //     nếu x == 1 hoặc x == n-1 -> qua; ngược lại bình phương (mulmod) tối đa s-1 lần,
    //     nếu gặp n-1 -> qua; nếu không -> hợp số.
    (void)ctx;
    (void)n;
    (void)rounds;
    return false;
}
