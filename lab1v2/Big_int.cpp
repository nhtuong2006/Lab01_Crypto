#include "Big_int.h"

void setZero(BigInt& a)
{
	for (int i = 0; i < MAX; i++)
	{
		a.data[i] = 0;
	}
}

BigInt fromUint(uint64_t v)
{
    BigInt r;
    setZero(r);
    r.data[0] = (uint32_t)v;
    r.data[1] = (uint32_t)(v >> 32);
    return r;
}

bool isZero(const BigInt& a)
{
	for (int i = 0; i < MAX; i++)
	{
		if (a.data[i] != 0)
			return false;
	}

	return true;
}

bool isOdd(const BigInt& a)
{
	return (a.data[0] & 1) == 1;
}

int bitLength(const BigInt& a)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a.data[i] != 0)
        {
            uint32_t x = a.data[i];
            int count = 0;

            while (x > 0)
            {
                count++;
                x >>= 1;
            }

            return i * 32 + count;
        }
    }

    return 0;
}

bool getBit(const BigInt& a, int i)
{
    int index = i / 32;
    int offset = i % 32;

    if (index >= MAX)
        return false;

    return (a.data[index] >> offset) & 1;
}

void setBit(BigInt& a, int i)
{
    int index = i / 32;
    int offset = i % 32;

    a.data[index] |= (1u << offset);
}

int compare(const BigInt& a, const BigInt& b)
{
    for (int i = MAX - 1; i >= 0; i--)
    {
        if (a.data[i] < b.data[i])
            return -1;

        if (a.data[i] > b.data[i])
            return 1;
    }

    return 0;
}

BigInt add(const BigInt& a, const BigInt& b)
{
    BigInt result = {};
    uint64_t carry = 0;

    for (int i = 0; i < MAX; i++)
    {
        uint64_t sum =
            (uint64_t)a.data[i] + b.data[i] + carry;

        result.data[i] = (uint32_t)sum;
        carry = sum >> 32;
    }

    return result;
}

BigInt subtract(const BigInt& a, const BigInt& b)
{
    BigInt result = {};
    uint64_t borrow = 0;

    for (int i = 0; i < MAX; i++)
    {
        uint64_t x = a.data[i];
        uint64_t y = (uint64_t)b.data[i] + borrow;

        result.data[i] = (uint32_t)(x - y);

        borrow = (x < y) ? 1 : 0;
    }

    return result;
}

void addSmall(BigInt& a, uint32_t b)
{
    uint64_t carry = b;

    for (int i = 0; i < MAX && carry > 0; i++)
    {
        uint64_t sum = (uint64_t)a.data[i] + carry;

        a.data[i] = (uint32_t)sum;
        carry = sum >> 32;
    }
}

void subtractSmall(BigInt& a, uint32_t b)
{
    uint64_t borrow = b;

    for (int i = 0; i < MAX && borrow > 0; i++)
    {
        uint64_t x = a.data[i];

        a.data[i] = (uint32_t)(x - borrow);
        borrow = (x < borrow) ? 1 : 0;
    }
}


void shiftLeftOne(BigInt& a)
{
    uint32_t carry = 0;

    for (int i = 0; i < MAX; i++)
    {
        uint32_t nextCarry = a.data[i] >> 31;

        a.data[i] = (a.data[i] << 1) | carry;

        carry = nextCarry;
    }
}

void shiftRightOne(BigInt& a)
{
    uint32_t carry = 0;

    for (int i = MAX - 1; i >= 0; i--)
    {
        uint32_t nextCarry = a.data[i] & 1u;

        a.data[i] = (a.data[i] >> 1) | (carry << 31);

        carry = nextCarry;
    }
}

uint32_t modSmall(const BigInt& a, uint32_t m)
{
    uint64_t rem = 0;

    for (int i = MAX - 1; i >= 0; i--)
    {
        rem = ((rem << 32) | a.data[i]) % m;
    }

    return (uint32_t)rem;
}

std::string toHex(const BigInt& a)
{
    const char* H = "0123456789abcdef";
    std::string s;

    for (int i = MAX - 1; i >= 0; i--)
        for (int k = 28; k >= 0; k -= 4)
            s += H[(a.data[i] >> k) & 15];

    size_t pos = s.find_first_not_of('0');
    if (pos == std::string::npos)
        return "0";

    return s.substr(pos);
}
