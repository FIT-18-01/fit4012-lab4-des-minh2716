#!/usr/bin/env bash
#!/usr/bin/env bash
# Test tamper negative: modify ciphertext and see decryption fails
set -euo pipefail

echo "Testing tamper negative..."

PLAINTEXT="Secret message"
KEY="0x133457799BBCDFF1"

# Build if needed
if [ ! -f ../des ]; then
    echo "Building des..."
    g++ -std=c++17 ../des.cpp -o ../des
fi

# Encrypt
ENCRYPT_OUTPUT=$(../des des encrypt $KEY "$PLAINTEXT")
CIPHERTEXT=$(echo "$ENCRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

# Tamper: flip one character
TAMPERED=$(echo "$CIPHERTEXT" | sed 's/./X/1')

# Decrypt tampered
DECRYPT_OUTPUT=$(../des des decrypt $KEY "$TAMPERED")
DECRYPTED=$(echo "$DECRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

if [ "$DECRYPTED" != "$PLAINTEXT" ]; then
    echo "Tamper negative test PASSED (decryption differs as expected)"
else
    echo "Tamper negative test FAILED (decryption should differ)"
    exit 1
fi
