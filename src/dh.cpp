#include "dh.h"
#include "primality.h"

uint512_t modular_exponentiation(const uint512_t &base, const uint512_t &exp, const uint512_t &mod)
{
    // TODO(A): square-and-multiply
    //   result = 1; b = base mod mod
    //   for i = 0 .. bit_length(exp)-1:
    //       if get_bit(exp, i): result = mulmod(result, b, mod)
    //       b = mulmod(b, b, mod)
    //   (dùng mod_reduce để đưa base về < mod trước khi lặp; mulmod là MultMod của thầy)
    (void)base;
    (void)exp;
    (void)mod;
    return uint512_t();
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
    // TODO: q = (p-1)/2 = shr1(p - 1). Thử g = 2,3,5,...
    //       chọn g đầu tiên thỏa g^2 mod p != 1 và g^q mod p != 1.
    (void)p;
    return uint512_t(2);
}
