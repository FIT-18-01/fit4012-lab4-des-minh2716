# Report 1 page - Lab 4 DES / TripleDES

## Mục tiêu

Triển khai thuật toán DES và TripleDES hoàn chỉnh, bao gồm mã hóa và giải mã, xử lý nhiều block với padding, và các test case.

## Cách làm / Method

Bổ sung vào code gốc:
- Hàm decrypt cho DES bằng cách đảo ngược round keys
- Lớp TripleDES với 3 khóa
- Xử lý input/output hex, binary, string
- Padding zero cho nhiều block
- Command line interface

Cấu trúc: KeyGenerator tạo round keys, DES thực hiện Feistel rounds, TripleDES kết hợp 3 DES.

## Kết quả / Result

Chạy test sample DES: encrypt 0x0123456789ABCDEF với key 0x133457799BBCDFF1 -> 0x85E813540F0AB405 ✓

Round-trip: "Hello World!" encrypt/decrypt -> khôi phục đúng ✓

Multi-block: message dài encrypt/decrypt -> đúng ✓

Negative tests: tamper và wrong key -> decryption sai như mong đợi ✓

## Kết luận / Conclusion

Hoàn thành DES và TripleDES với decrypt, multi-block, padding. Hạn chế: zero padding không an toàn, không hỗ trợ mode như CBC. Mở rộng: thêm CBC mode, PKCS7 padding, file I/O.
