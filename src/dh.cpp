#include "dh.h"
#include "primality.h"

uint512_t modular_exponentiation(const uint512_t &base, const uint512_t &exp, const uint512_t &mod)
{
    // TODO(A): square-and-multiply
    //   result = 1; b = base mod mod
    uint512_t result = mod_reduce(uint512_t(1), mod); // 1 mod m (bằng 0 nếu m = 1)
    uint512_t b = mod_reduce(base, mod);              // Rút gọn cơ số trước khi tính toán

    //   for i = 0 .. bit_length(exp)-1:
    size_t len = bit_length(exp);
    for (size_t i = 0; i < len; ++i)
    {
        //   if get_bit(exp, i): result = mulmod(result, b, mod)
        if (get_bit(exp, i))
        {
            result = mulmod(result, b, mod);
        }
        //   b = mulmod(b, b, mod)
        b = mulmod(b, b, mod);
    }
    return result;
}

uint512_t generate_safe_prime(prng_ctx *ctx, size_t bitsize)
{
    // TODO(B):
    //   loop:
    //     q = random_bits(ctx, bitsize - 1, true)     // q có bitsize-1 bit, lẻ
    //     p = 2q + 1  (shl1(q) + 1)                    // p có đúng bitsize bit
    //     lọc nhanh: với mỗi số nguyên tố nhỏ r: q % r != 0 và p % r != 0
    //     Miller-Rabin ít vòng cho q, rồi cho p; qua cả hai thì xác nhận lại nhiều vòng hơn
    //   return p
    // Mẹo tăng tốc: thay vì sinh lại hoàn toàn, tăng q thêm 2 và lọc tiếp.
    (void)ctx;
    (void)bitsize;
    return uint512_t();
}

uint512_t generate_private_key(prng_ctx *ctx, const uint512_t &p)
{
    // TODO(B): return random_range(ctx, 2, p - 2)
    (void)ctx;
    (void)p;
    return uint512_t();
}

uint512_t find_generator(const uint512_t &p)
{
    // TODO: q = (p-1)/2 = shr1(p - 1).
    uint512_t p_minus_1 = p - uint512_t(1);
    uint512_t q = shr1(p_minus_1);

    // Thử g = 2,3,5,...
    // chọn g đầu tiên thỏa g^2 mod p != 1 và g^q mod p != 1.
    // Duyệt g từ 2 trở đi để thử
    uint64_t candidate = 2;
    while (true)
    {
        uint512_t g(candidate);

        uint512_t g_squared = modular_exponentiation(g, uint512_t(2), p);
        uint512_t g_q = modular_exponentiation(g, q, p);

        // Nếu thỏa mãn điều kiện thì đó chính là generator
        if (g_squared != uint512_t(1) && g_q != uint512_t(1))
        {
            return g;
        }
        candidate++;
    }
    return uint512_t(2); // Fallback an toàn, dù hiếm khi chạy đến đây
}
