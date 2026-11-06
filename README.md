# Lab01 – Số nguyên tố & Diffie-Hellman (nhóm 2 người)

## Cấu trúc
```
src/bigint.{h,cpp}      A: số lớn uint512_t (cộng, trừ, so sánh, dịch bit, mod_reduce, addmod, mulmod)
src/dh.{h,cpp}          A: modular_exponentiation | B: generate_safe_prime, generate_private_key | find_generator
src/primality.{h,cpp}   B: random_bits, random_range, mod_small, Miller-Rabin
src/main.cpp            Chạy trao đổi khóa Diffie-Hellman và in kết quả
tests/                  test_bigint (A), test_prime (B)
tools/check_dh.py       Kiểm tra độc lập bằng Python (chỉ dùng để test)
lib/fips202/            PRNG SHAKE128 do giảng viên cung cấp
```
Tìm `TODO(A)` / `TODO(B)` trong code để biết chỗ cần làm.

## Lệnh
```
make               # biên dịch bin/dh (C++14)
make test-bigint   # test phần A
make test-prime    # test phần B (cần A xong trước)
make run           # chạy ./bin/dh  (tùy chọn: ./bin/dh 512 <seed>)
make check         # chạy rồi cho Python kiểm tra p, g, A, B, bí mật chung
```

## Thứ tự làm đề xuất
1. A: cmp → cộng/trừ → dịch bit → mod_reduce → addmod → mulmod (MultMod của thầy) → modular_exponentiation → `make test-bigint`.
2. B (song song): mod_small → random_bits → random_range (test được ngay, chỉ cần `cmp` của A cho vài kiểm tra).
3. B: is_probable_prime (cần modular_exponentiation của A) → generate_safe_prime → generate_private_key.
4. Cả hai: find_generator, chạy `make check`, rồi viết báo cáo.

## Thay đổi độ lớn số nguyên tố
Hiện `uint512_t` có 8 limb 64 bit, nên bit_size tối đa là 512 (đề yêu cầu tối thiểu 512 bit).
Muốn 1024 bit: đổi số limb (8 → 16) và kiểu rộng (16 → 32) rồi sửa các vòng lặp, giới hạn trong `main.cpp`.
Ghi lại cách chỉnh vào báo cáo (mục 5).

## Nộp bài
```
<MSSV1-MSSV2>.zip
  project_01_source/   (thư mục này, bỏ bin/)
  project_01_report/   (report.pdf)
```
Trước khi nộp: `make clean && make` từ thư mục mới giải nén, không còn warning/lỗi.
