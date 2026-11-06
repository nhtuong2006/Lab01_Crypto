// Lab01 - Số nguyên tố & Diffie-Hellman
// Cách chạy: ./bin/dh [bit_size] [seed_cố_định]
//   bit_size: mặc định 512 (tối thiểu 512 theo đề)
//   có đối số seed -> dùng seed cố định (để tái lập), không có -> seed từ hệ điều hành

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <string>
#include "dh.h"
#include "primality.h"

int main(int argc, char **argv)
{
    size_t bit_size = 512;
    if (argc >= 2)
        bit_size = (size_t)atoi(argv[1]);
    if (bit_size < 512 || bit_size > 512)
    {
        // Hiện tại uint512_t chỉ chứa tối đa 512 bit.
        // Muốn lớn hơn: đổi số limb (xem README, mục "Thay đổi độ lớn").
        fprintf(stderr, "bit_size phải bằng 512 với uint512_t (nhận %zu)\n", bit_size);
        return 1;
    }

    uint8_t seed[32] = {0};
    prng_ctx ctx;
    if (argc >= 3)
    {
        strncpy((char *)seed, argv[2], sizeof(seed) - 1);
    }
    else
    {
        randombytes(seed, sizeof(seed));
    }
    prng_seed(&ctx, seed, sizeof(seed));

    // 1. p, g
    uint512_t p = generate_safe_prime(&ctx, bit_size);
    if (is_zero(p))
    {
        fprintf(stderr, "generate_safe_prime chưa được cài đặt (trả về 0)\n");
        return 2;
    }
    uint512_t g = find_generator(p);

    // 2. khóa riêng
    uint512_t a = generate_private_key(&ctx, p);
    uint512_t b = generate_private_key(&ctx, p);

    // 3. khóa công khai
    uint512_t A = modular_exponentiation(g, a, p);
    uint512_t B = modular_exponentiation(g, b, p);

    // 4. bí mật chung
    uint512_t sa = modular_exponentiation(B, a, p);
    uint512_t sb = modular_exponentiation(A, b, p);

    // 5. in kết quả (định dạng "tên=hex" để tools/check_dh.py đọc được)
    printf("p=%s\n", to_hex(p).c_str());
    printf("g=%s\n", to_hex(g).c_str());
    printf("a=%s\n", to_hex(a).c_str());
    printf("b=%s\n", to_hex(b).c_str());
    printf("A=%s\n", to_hex(A).c_str());
    printf("B=%s\n", to_hex(B).c_str());
    printf("alice_secret=%s\n", to_hex(sa).c_str());
    printf("bob_secret=%s\n", to_hex(sb).c_str());
    printf("match=%d\n", (int)(sa == sb));
    return sa == sb ? 0 : 3;
}
