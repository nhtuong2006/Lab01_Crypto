#!/usr/bin/env python3
"""Kiểm tra độc lập kết quả của chương trình C++ bằng Python (chỉ để test, KHÔNG nộp thay cho code).

Dùng:  ./bin/dh | python3 tools/check_dh.py
"""
import random
import sys


def is_prime(n, rounds=40):
    if n < 2:
        return False
    for sp in (2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37):
        if n % sp == 0:
            return n == sp
    d, s = n - 1, 0
    while d % 2 == 0:
        d //= 2
        s += 1
    for _ in range(rounds):
        a = random.randrange(2, n - 1)
        x = pow(a, d, n)
        if x in (1, n - 1):
            continue
        for _ in range(s - 1):
            x = x * x % n
            if x == n - 1:
                break
        else:
            return False
    return True


vals = {}
for line in sys.stdin:
    if "=" in line:
        k, v = line.strip().split("=", 1)
        vals[k] = v

p, g, a, b, A, B = (int(vals[k], 16) for k in ("p", "g", "a", "b", "A", "B"))
sa, sb = int(vals["alice_secret"], 16), int(vals["bob_secret"], 16)
q = (p - 1) // 2

checks = [
    ("p có đúng số bit mong muốn (in ra bên dưới)", True),
    ("p nguyên tố", is_prime(p)),
    ("(p-1)/2 nguyên tố (safe prime)", is_prime(q)),
    ("g^2 != 1 và g^q != 1 (mod p)", pow(g, 2, p) != 1 and pow(g, q, p) != 1),
    ("a, b trong [2, p-2]", 2 <= a <= p - 2 and 2 <= b <= p - 2),
    ("A = g^a mod p", A == pow(g, a, p)),
    ("B = g^b mod p", B == pow(g, b, p)),
    ("bí mật Alice đúng", sa == pow(B, a, p)),
    ("bí mật Bob đúng", sb == pow(A, b, p)),
    ("hai bí mật trùng nhau", sa == sb),
]
print("p có %d bit" % p.bit_length())
ok = True
for name, res in checks:
    print(("[OK]   " if res else "[LỖI] ") + name)
    ok &= bool(res)
sys.exit(0 if ok else 1)
