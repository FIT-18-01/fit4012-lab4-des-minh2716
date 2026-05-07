#!/usr/bin/env bash
# Test DES sample encryption
set -euo pipefail

echo "Testing DES sample encryption..."

# Standard test vector:
# Plaintext: 0x0123456789ABCDEF
# Key: 0x133457799BBCDFF1
# Ciphertext: 0x85E813540F0AB405

EXPECTED="0x85E813540F0AB405"

# Build if needed
if [ ! -f ../des ]; then
    echo "Building des..."
    g++ -std=c++17 ../des.cpp -o ../des
fi

# Run encryption
OUTPUT=$(../des des encrypt 0x133457799BBCDFF1 0x0123456789ABCDEF)

# Extract the result
RESULT=$(echo "$OUTPUT" | grep "Result (hex):" | cut -d' ' -f3)

if [ "$RESULT" = "$EXPECTED" ]; then
    echo "DES sample test PASSED"
else
    echo "DES sample test FAILED"
    echo "Expected: $EXPECTED"
    echo "Got: $RESULT"
    exit 1
fi
