#ifndef BIG_INT_H
#define BIG_INT_H

#include <cstdint>
#include <string>

// So limb (moi limb 32 bit). Can chua duoc bit_size + 1 bit (vi mulmod co buoc nhan doi).
//   512 bit  -> MAX >= 17
//  1024 bit  -> MAX >= 33
const int MAX = 17;

struct BigInt
{
    uint32_t data[MAX];   // data[0] la limb thap nhat
};

// Khoi tao so 0
void setZero(BigInt& a);

// Tao BigInt tu so 64 bit
BigInt fromUint(uint64_t v);

// Kiem tra so 0
bool isZero(const BigInt& a);

// Kiem tra so le
bool isOdd(const BigInt& a);

// Lay so bit cua so nguyen
int bitLength(const BigInt& a);

// Lay bit tai vi tri i
bool getBit(const BigInt& a, int i);

// Dat bit tai vi tri i
void setBit(BigInt& a, int i);

// So sanh hai so: -1 (a<b), 0 (bang), 1 (a>b)
int compare(const BigInt& a, const BigInt& b);

// a + b
BigInt add(const BigInt& a, const BigInt& b);

// a - b (yeu cau a >= b)
BigInt subtract(const BigInt& a, const BigInt& b);

// a += b (b la so nho)
void addSmall(BigInt& a, uint32_t b);

// a -= b (b la so nho, yeu cau a >= b)
void subtractSmall(BigInt& a, uint32_t b);

// Dich trai 1 bit
void shiftLeftOne(BigInt& a);

// Dich phai 1 bit
void shiftRightOne(BigInt& a);

// a mod m, voi m la so nho (dung de sang so nguyen to nho)
uint32_t modSmall(const BigInt& a, uint32_t m);

// Chuyen sang chuoi hex de in
std::string toHex(const BigInt& a);

#endif
