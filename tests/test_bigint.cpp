// Test cho phần của bạn A. Chạy: make test-bigint
// Các phép so sánh bằng dùng chuỗi hex (EQ) để không phụ thuộc vào hàm cmp/operator== đang được kiểm tra.
// Giá trị mong đợi đã được đối chiếu bằng Python.
#include <stdio.h>
#include <string>
#include "dh.h"
#include "bigint.h"

static int fails = 0;
#define CHECK(cond)                                          \
    do                                                       \
    {                                                        \
        if (!(cond))                                         \
        {                                                    \
            printf("  FAIL line %d: %s\n", __LINE__, #cond); \
            fails++;                                         \
        }                                                    \
    } while (0)
#define EQ(expr, hex) CHECK(to_hex(expr) == std::string(hex))

static uint512_t H(const char *s)
{
    uint512_t r;
    from_hex(s, r);
    return r;
}

int main()
{
    printf("[cmp / toán tử so sánh]\n");
    CHECK(cmp(H("5"), H("7")) < 0);
    CHECK(cmp(H("7"), H("5")) > 0);
    CHECK(cmp(H("ffffffffffffffff0"), H("ffffffffffffffff")) > 0);
    CHECK(cmp(H("10000000000000000"), H("ffffffffffffffff")) > 0);
    CHECK(cmp(H("10000000000000000"), H("10000000000000000")) == 0);
    CHECK(H("5") < H("7"));
    CHECK(H("1") <= H("1"));
    CHECK(!(H("7") < H("5")));

    printf("[cộng / trừ, có nhớ qua limb]\n");
    EQ(H("ffffffffffffffff") + H("1"), "10000000000000000");
    EQ(H("10000000000000000") - H("1"), "ffffffffffffffff");
    EQ(H("123456789abcdef0123456789abcdef") + H("fedcba9876543210fedcba987654321"),
       "11111111111111101111111111111110");

    printf("[dịch bit]\n");
    EQ(shl1(H("8000000000000000")), "10000000000000000");
    EQ(shr1(H("10000000000000000")), "8000000000000000");
    EQ(shl1(H("3")), "6");
    EQ(shr1(H("7")), "3");

    printf("[mod_reduce]\n");
    EQ(mod_reduce(H("64"), H("7")), "2");                              // 100 mod 7
    EQ(mod_reduce(H("5"), H("7")), "5");
    EQ(mod_reduce(H("ffffffffffffffffffffffffffffffff"), H("1000000000000000d")), "a8");

    printf("[addmod, gồm cả trường hợp x+y tràn 512 bit]\n");
    EQ(addmod(H("5"), H("4"), H("7")), "2");
    EQ(addmod(H("3"), H("3"), H("7")), "6");
    {
        uint512_t p = H("ffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffff"); // 2^512-1 (128 chữ f)
        EQ(addmod(p - H("1"), p - H("1"), p), "fffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffffd");
    }

    printf("[mulmod]\n");
    EQ(mulmod(H("ffffffffffffffff"), H("ffffffffffffffff"), H("1000000000000000d")), "c4");
    EQ(mulmod(H("7"), H("9"), H("a")), "3");
    EQ(mulmod(H("0"), H("9"), H("a")), "0");
    {
        // (p-1)*(p-1) mod p = 1 với p = 2^127-1
        uint512_t p = H("7fffffffffffffffffffffffffffffff");
        EQ(mulmod(p - H("1"), p - H("1"), p), "1");
    }

    printf("[modular_exponentiation]\n");
    EQ(modular_exponentiation(H("2"), H("a"), H("3e8")), "18"); // 2^10 mod 1000 = 24
    EQ(modular_exponentiation(H("3"), H("0"), H("7")), "1");
    {
        // Fermat: a^(p-1) = 1 mod p, p = 2^127-1
        uint512_t p = H("7fffffffffffffffffffffffffffffff");
        EQ(modular_exponentiation(H("12345"), p - H("1"), p), "1");
    }

    printf(fails ? "=> %d lỗi\n" : "=> TẤT CẢ ĐỀU QUA\n", fails);
    return fails ? 1 : 0;
}
