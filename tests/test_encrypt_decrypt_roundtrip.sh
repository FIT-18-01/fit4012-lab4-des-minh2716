#!/usr/bin/env bash
#!/usr/bin/env bash
# Test round-trip encrypt -> decrypt
set -euo pipefail

echo "Testing DES round-trip..."

PLAINTEXT="Hello World!"
KEY="0x133457799BBCDFF1"

# Build if needed
if [ ! -f ../des ]; then
    echo "Building des..."
    g++ -std=c++17 ../des.cpp -o ../des
fi

# Encrypt
ENCRYPT_OUTPUT=$(../des des encrypt $KEY "$PLAINTEXT")
CIPHERTEXT=$(echo "$ENCRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

# Decrypt
DECRYPT_OUTPUT=$(../des des decrypt $KEY "$CIPHERTEXT")
DECRYPTED=$(echo "$DECRYPT_OUTPUT" | grep "Result:" | cut -d' ' -f2-)

if [ "$DECRYPTED" = "$PLAINTEXT" ]; then
    echo "DES round-trip test PASSED"
else
    echo "DES round-trip test FAILED"
    echo "Original: $PLAINTEXT"
    echo "Decrypted: $DECRYPTED"
    exit 1
fi
