#include "bigint.h"

// ---------- constructors ----------
uint512_t::uint512_t()
{
    for (size_t i = 0; i < 8; i++)
        digits[i] = 0;
}

uint512_t::uint512_t(uint64_t v)
{
    for (size_t i = 0; i < 8; i++)
        digits[i] = 0;
    digits[0] = v;
}

// ---------- tiện ích đã cài sẵn ----------
bool is_zero(const uint512_t &a)
{
    for (size_t i = 0; i < 8; i++)
        if (a.digits[i] != 0)
            return false;
    return true;
}

size_t bit_length(const uint512_t &a)
{
    for (size_t i = 8; i-- > 0;)
    {
        if (a.digits[i] != 0)
        {
            size_t n = 0;
            uint64_t v = a.digits[i];
            while (v)
            {
                n++;
                v >>= 1;
            }
            return i * 64 + n;
        }
    }
    return 0;
}

bool get_bit(const uint512_t &a, size_t i)
{
    if (i >= 512)
        return false;
    return (a.digits[i / 64] >> (i % 64)) & 1;
}

void set_bit(uint512_t &a, size_t i)
{
    if (i < 512)
        a.digits[i / 64] |= (uint64_t)1 << (i % 64);
}

std::string to_hex(const uint512_t &a)
{
    static const char *H = "0123456789abcdef";
    std::string s;
    bool started = false;
    for (size_t i = 8; i-- > 0;)
    {
        for (int sh = 60; sh >= 0; sh -= 4)
        {
            unsigned d = (unsigned)((a.digits[i] >> sh) & 0xF);
            if (d != 0)
                started = true;
            if (started)
                s.push_back(H[d]);
        }
    }
    return s.empty() ? "0" : s;
}

bool from_hex(const std::string &s, uint512_t &out)
{
    out = uint512_t();
    if (s.empty() || s.size() > 128)
        return false;
    size_t bit = 0;
    for (size_t k = s.size(); k-- > 0; bit += 4)
    {
        char c = s[k];
        unsigned d;
        if (c >= '0' && c <= '9')
            d = c - '0';
        else if (c >= 'a' && c <= 'f')
            d = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')
            d = c - 'A' + 10;
        else
            return false;
        out.digits[bit / 64] |= (uint64_t)d << (bit % 64);
    }
    return true;
}

// ============================================================
//  TODO(A): cài các hàm dưới đây
// ============================================================

int cmp(const uint512_t &a, const uint512_t &b)
{
    // TODO(A): so sánh từ limb cao xuống thấp
    (void)a;
    (void)b;
    return 0;
}

uint512_t operator+(const uint512_t &a, const uint512_t &b)
{
    // TODO(A): cộng từng limb, nhớ carry. Gợi ý: dùng uint128_t hoặc kiểm tra tràn
    (void)a;
    (void)b;
    return uint512_t();
}

uint512_t operator-(const uint512_t &a, const uint512_t &b)
{
    // TODO(A): trừ từng limb, nhớ borrow
    (void)a;
    (void)b;
    return uint512_t();
}

uint512_t shl1(const uint512_t &a)
{
    // TODO(A): dịch trái 1 bit, bit cao của limb dưới chuyển lên limb trên
    (void)a;
    return uint512_t();
}

uint512_t shr1(const uint512_t &a)
{
    // TODO(A): dịch phải 1 bit
    (void)a;
    return uint512_t();
}

uint512_t mod_reduce(const uint512_t &x, const uint512_t &m)
{
    // TODO(A): x mod m. Cách dễ: duyệt bit của x từ cao xuống thấp (shift-and-subtract):
    //   r = 0; với mỗi bit: r = 2r + bit; nếu r >= m thì r -= m.
    //   Lưu ý: 2r có thể tràn 512 bit khi m gần 2^512 -> ghi nhớ bit cao trước khi dịch.
    // Nếu x < m thì trả về x ngay.
    (void)x;
    (void)m;
    return uint512_t();
}

uint512_t addmod(const uint512_t &x, const uint512_t &y, const uint512_t &m)
{
    // TODO(A): theo bảng của thầy (giả sử x, y đã < m):
    //   if (x >= m - y) return x - (m - y);
    //   return x + y;
    // vì m - y luôn vừa 512 bit nên không bao giờ tràn.
    (void)x;
    (void)y;
    (void)m;
    return uint512_t();
}

uint512_t mulmod(const uint512_t &x, const uint512_t &y, const uint512_t &m)
{
    // TODO(A): MultMod của thầy (double-and-add)
    //   x = mod_reduce(x, m)
    //   ret = bit0(y) ? x : 0
    //   for i = 1 .. bit_length(y)-1:
    //       x = addmod(x, x, m)                  // x <- 2x mod m
    //       if get_bit(y, i): ret = addmod(ret, x, m)
    //   return ret
    (void)x;
    (void)y;
    (void)m;
    return uint512_t();
}

// ---------- toán tử so sánh ----------
bool operator==(const uint512_t &a, const uint512_t &b) { return cmp(a, b) == 0; }
bool operator!=(const uint512_t &a, const uint512_t &b) { return cmp(a, b) != 0; }
bool operator<(const uint512_t &a, const uint512_t &b) { return cmp(a, b) < 0; }
bool operator>(const uint512_t &a, const uint512_t &b) { return cmp(a, b) > 0; }
bool operator<=(const uint512_t &a, const uint512_t &b) { return cmp(a, b) <= 0; }
bool operator>=(const uint512_t &a, const uint512_t &b) { return cmp(a, b) >= 0; }
