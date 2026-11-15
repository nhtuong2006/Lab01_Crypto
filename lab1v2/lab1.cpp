// Lab 01 - ket hop BigInt cua nhom (Big_int.h / Big_int.cpp) voi Miller-Rabin,
// sinh so nguyen to an toan, sinh khoa rieng va Diffie-Hellman.
// Bien dich: g++ -std=c++14 -O2 lab1.cpp Big_int.cpp -o lab1
// Chay:      ./lab1 512
#include <iostream>
#include <random>
#include <cstdlib>
#include "Big_int.h"

using namespace std;

// ===== SO NGAU NHIEN =================================================
random_device rd;   // nguon ngau nhien cua he dieu hanh

// So ngau nhien co dung 'bits' bit (bit cao nhat = 1)
BigInt randomBits(int bits)
{
    BigInt r;
    setZero(r);

    int limbs = (bits + 31) / 32;
    for (int i = 0; i < limbs; i++)
        r.data[i] = rd();

    int extra = limbs * 32 - bits;
    if (extra > 0)
        r.data[limbs - 1] &= (0xFFFFFFFFu >> extra);   // cat bot bit thua

    setBit(r, bits - 1);                               // bat bit cao nhat
    return r;
}

// So ngau nhien deu trong [0, n): sinh roi bo neu >= n (khong dung %)
BigInt randomBelow(const BigInt& n)
{
    int bits = bitLength(n);
    int limbs = (bits + 31) / 32;

    while (true)
    {
        BigInt r;
        setZero(r);

        for (int i = 0; i < limbs; i++)
            r.data[i] = rd();

        int extra = limbs * 32 - bits;
        if (extra > 0)
            r.data[limbs - 1] &= (0xFFFFFFFFu >> extra);

        if (compare(r, n) < 0)
            return r;
    }
}

// ===== LUY THUA MO-DUN (phan A) ======================================
// (a*b) mod m chi bang cong + tru + so sanh. Yeu cau a, b < m.
// Duyet bit cua b tu cao xuong thap: r = 2r ; neu bit = 1 thi r = r + a ;
// moi lan deu tru bot m neu r >= m  ->  r luon < m.
BigInt mulmod(const BigInt& a, const BigInt& b, const BigInt& m)
{
    BigInt r;
    setZero(r);

    for (int i = bitLength(b) - 1; i >= 0; i--)
    {
        shiftLeftOne(r);
        if (compare(r, m) >= 0)
            r = subtract(r, m);

        if (getBit(b, i))
        {
            r = add(r, a);
            if (compare(r, m) >= 0)
                r = subtract(r, m);
        }
    }

    return r;
}

// base^exponent mod m (binh phuong & nhan). Yeu cau base < m.
BigInt modular_exponentiation(BigInt base, const BigInt& exponent, const BigInt& m)
{
    BigInt result = fromUint(1);
    int nbits = bitLength(exponent);

    for (int i = 0; i < nbits; i++)
    {
        if (getBit(exponent, i))
            result = mulmod(result, base, m);

        base = mulmod(base, base, m);
    }

    return result;
}

// ===== MILLER-RABIN (phan B) =========================================
const uint32_t SMALL[] = { 2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97 };
const int NUM_SMALL = sizeof(SMALL) / sizeof(SMALL[0]);

// true neu n "co the la" nguyen to (sai voi xac suat <= 4^-rounds)
bool Miller_Rabin(const BigInt& n, int rounds)
{
    // 1. Chia thu cho cac so nguyen to nho
    for (int i = 0; i < NUM_SMALL; i++)
    {
        if (compare(n, fromUint(SMALL[i])) == 0)
            return true;
        if (modSmall(n, SMALL[i]) == 0)
            return false;
    }

    // 2. Viet n-1 = 2^s * d, d le
    BigInt one = fromUint(1);
    BigInt n1 = n;
    subtractSmall(n1, 1);               // n1 = n - 1

    BigInt d = n1;
    int s = 0;
    while (!isOdd(d))
    {
        shiftRightOne(d);
        s++;
    }

    BigInt n3 = n;
    subtractSmall(n3, 3);               // n3 = n - 3

    // 3. Thu 'rounds' co so a ngau nhien trong [2, n-2]
    for (int r = 0; r < rounds; r++)
    {
        BigInt a = randomBelow(n3);
        addSmall(a, 2);

        BigInt x = modular_exponentiation(a, d, n);

        if (compare(x, one) == 0 || compare(x, n1) == 0)
            continue;

        bool composite = true;
        for (int i = 1; i < s; i++)
        {
            x = mulmod(x, x, n);
            if (compare(x, n1) == 0)
            {
                composite = false;
                break;
            }
        }

        if (composite)
            return false;               // chac chan la hop so
    }

    return true;                        // co the la nguyen to
}

// So nguyen to an toan: p = 2q + 1, trong do p va q deu nguyen to
BigInt generate_safe_prime(int bit_size)
{
    while (true)
    {
        BigInt q = randomBits(bit_size - 1);
        q.data[0] |= 1;                 // q le

        // Sang nhanh: p = 2q+1 chia het cho r  <=>  q mod r = (r-1)/2
        bool ok = true;
        for (int i = 1; i < NUM_SMALL; i++)
        {
            uint32_t m = modSmall(q, SMALL[i]);
            if (m == 0 || m == (SMALL[i] - 1) / 2)
            {
                ok = false;
                break;
            }
        }
        if (!ok)
            continue;

        BigInt p = q;
        shiftLeftOne(p);
        addSmall(p, 1);                 // p = 2q + 1

        if (!Miller_Rabin(q, 1) || !Miller_Rabin(p, 1))     // thu nhanh
            continue;

        if (Miller_Rabin(q, 20) && Miller_Rabin(p, 20))     // thu ky
            return p;
    }
}

// ===== KHOA RIENG trong [2, p-2] (phan C) ============================
BigInt generate_private_key(const BigInt& p)
{
    BigInt range = p;
    subtractSmall(range, 3);            // p - 3
    BigInt k = randomBelow(range);      // [0, p-3)
    addSmall(k, 2);                     // [2, p-2]
    return k;
}

// ===== DIFFIE-HELLMAN ================================================
int main(int argc, char** argv)
{
    int bit_size = (argc > 1) ? atoi(argv[1]) : 512;

    if (bit_size < 32 || bit_size + 1 > MAX * 32)
    {
        cout << "bit_size khong hop le. Hay tang MAX trong Big_int.h (MAX*32 >= bit_size+1)\n";
        return 1;
    }

    cout << "Dang sinh so nguyen to an toan " << bit_size << " bit...\n";
    BigInt p = generate_safe_prime(bit_size);
    BigInt g = fromUint(2);
    cout << "p = " << toHex(p) << "\n";

    BigInt a = generate_private_key(p);
    BigInt b = generate_private_key(p);

    BigInt A = modular_exponentiation(g, a, p);
    BigInt B = modular_exponentiation(g, b, p);

    BigInt alice = modular_exponentiation(B, a, p);
    BigInt bob   = modular_exponentiation(A, b, p);

    cout << "a = " << toHex(a) << "\nb = " << toHex(b) << "\n";
    cout << "A = " << toHex(A) << "\nB = " << toHex(B) << "\n";
    cout << "Alice: " << toHex(alice) << "\nBob:   " << toHex(bob) << "\n";
    cout << "Trung khop? " << (compare(alice, bob) == 0 ? "DUNG" : "SAI") << "\n";
    return 0;
}
