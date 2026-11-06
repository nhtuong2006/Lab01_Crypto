// Test cho phần của bạn B. Chạy: make test-prime
// (cần phần của A hoạt động: Miller-Rabin dùng modular_exponentiation)
#include <stdio.h>
#include <string.h>
#include "dh.h"
#include "primality.h"

static int fails = 0;
#define CHECK(cond)                                              \
    do                                                           \
    {                                                            \
        if (!(cond))                                             \
        {                                                        \
            printf("  FAIL line %d: %s\n", __LINE__, #cond);     \
            fails++;                                             \
        }                                                        \
    } while (0)

static uint512_t H(const char *s)
{
    uint512_t r;
    from_hex(s, r);
    return r;
}

int main()
{
    uint8_t seed[32] = {0};
    memcpy(seed, "test-seed", 9);
    prng_ctx ctx;
    prng_seed(&ctx, seed, sizeof(seed));

    printf("[mod_small]\n");
    CHECK(mod_small(H("64"), 7) == 2);                       // 100 mod 7
    CHECK(mod_small(H("10000000000000000"), 1000) == 616);   // 2^64 mod 1000

    printf("[random_bits]\n");
    for (int i = 0; i < 50; i++)
    {
        uint512_t r = random_bits(&ctx, 200, true);
        CHECK(bit_length(r) == 200);
        CHECK(get_bit(r, 0));
    }

    printf("[random_range]\n");
    for (int i = 0; i < 200; i++)
    {
        uint512_t r = random_range(&ctx, H("2"), H("a"));
        CHECK(r >= H("2") && r <= H("a"));
    }

    printf("[is_probable_prime]\n");
    CHECK(is_probable_prime(&ctx, H("2")));
    CHECK(is_probable_prime(&ctx, H("61")));                               // 97
    CHECK(!is_probable_prime(&ctx, H("1")));
    CHECK(!is_probable_prime(&ctx, H("0")));
    CHECK(!is_probable_prime(&ctx, H("231")));                             // 561 = Carmichael
    CHECK(!is_probable_prime(&ctx, H("64")));                              // 100
    CHECK(is_probable_prime(&ctx, H("7fffffffffffffffffffffffffffffff")));  // 2^127-1
    CHECK(!is_probable_prime(&ctx, H("ffffffffffffffffffffffffffffffff"))); // 2^128-1

    printf("[generate_safe_prime 128 bit (nhanh)]\n");
    {
        uint512_t p = generate_safe_prime(&ctx, 128);
        CHECK(bit_length(p) == 128);
        CHECK(is_probable_prime(&ctx, p, 30));
        CHECK(is_probable_prime(&ctx, shr1(p - uint512_t(1)), 30));
    }

    printf("[generate_private_key]\n");
    {
        uint512_t p = H("7fffffffffffffffffffffffffffffff");
        for (int i = 0; i < 100; i++)
        {
            uint512_t k = generate_private_key(&ctx, p);
            CHECK(k >= H("2") && k <= p - H("2"));
        }
    }

    printf(fails ? "=> %d lỗi\n" : "=> TẤT CẢ ĐỀU QUA\n", fails);
    return fails ? 1 : 0;
}
