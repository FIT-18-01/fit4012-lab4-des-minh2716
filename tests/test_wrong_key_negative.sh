#!/usr/bin/env bash
#!/usr/bin/env bash
# Test wrong key negative: decrypt with wrong key
set -euo pipefail

echo "Testing wrong key negative..."

PLAINTEXT="Secret message"
KEY="0x133457799BBCDFF1"
WRONG_KEY="0x133457799BBCDFF2"

# Build if needed
if [ ! -f ../des ]; then
    echo "Building des..."
    g++ -std=c++17 ../des.cpp -o ../des
fi

# Encrypt with correct key
ENCRYPT_OUTPUT=$(../des des encrypt $KEY "$PLAINTEXT")
CIPHERTEXT=$(echo "$ENCRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

# Decrypt with wrong key
DECRYPT_OUTPUT=$(../des des decrypt $WRONG_KEY "$CIPHERTEXT")
DECRYPTED=$(echo "$DECRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

if [ "$DECRYPTED" != "$PLAINTEXT" ]; then
    echo "Wrong key negative test PASSED (decryption differs as expected)"
else
    echo "Wrong key negative test FAILED (decryption should differ)"
    exit 1
fi
