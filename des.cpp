#include <algorithm>
#include <bitset>
#include <cctype>
#include <iostream>
#include <string>
#include <vector>

using namespace std;

string convert_decimal_to_binary(int decimal) {
    return bitset<4>(decimal).to_string();
}

int convert_binary_to_decimal(const string& binary) {
    return stoi(binary, nullptr, 2);
}

string Xor(const string& a, const string& b) {
    string result;
    result.reserve(a.size());
    for (size_t i = 0; i < a.size(); ++i) {
        result += (a[i] != b[i]) ? '1' : '0';
    }
    return result;
}

string initial_permutation(const string& input) {
    const int table[64] = {
        58,50,42,34,26,18,10,2,
        60,52,44,36,28,20,12,4,
        62,54,46,38,30,22,14,6,
        64,56,48,40,32,24,16,8,
        57,49,41,33,25,17,9,1,
        59,51,43,35,27,19,11,3,
        61,53,45,37,29,21,13,5,
        63,55,47,39,31,23,15,7
    };

    string permuted;
    permuted.reserve(64);
    for (int i = 0; i < 64; ++i) permuted += input[table[i] - 1];
    return permuted;
}

string inverse_initial_permutation(const string& input) {
    const int table[64] = {
        40,8,48,16,56,24,64,32,
        39,7,47,15,55,23,63,31,
        38,6,46,14,54,22,62,30,
        37,5,45,13,53,21,61,29,
        36,4,44,12,52,20,60,28,
        35,3,43,11,51,19,59,27,
        34,2,42,10,50,18,58,26,
        33,1,41,9,49,17,57,25
    };

    string permuted;
    permuted.reserve(64);
    for (int i = 0; i < 64; ++i) permuted += input[table[i] - 1];
    return permuted;
}

class KeyGenerator {
private:
    string key;
    vector<string> roundKeys;

    const int pc1[56] = {
        57,49,41,33,25,17,9,
        1,58,50,42,34,26,18,
        10,2,59,51,43,35,27,
        19,11,3,60,52,44,36,
        63,55,47,39,31,23,15,
        7,62,54,46,38,30,22,
        14,6,61,53,45,37,29,
        21,13,5,28,20,12,4
    };

    const int pc2[48] = {
        14,17,11,24,1,5,
        3,28,15,6,21,10,
        23,19,12,4,26,8,
        16,7,27,20,13,2,
        41,52,31,37,47,55,
        30,40,51,45,33,48,
        44,49,39,56,34,53,
        46,42,50,36,29,32
    };

    string shift_left_once(const string& key_chunk) {
        return key_chunk.substr(1) + key_chunk[0];
    }

    string shift_left_twice(string key_chunk) {
        key_chunk = shift_left_once(key_chunk);
        return shift_left_once(key_chunk);
    }

public:
    explicit KeyGenerator(const string& input_key) : key(input_key) {}

    void generateRoundKeys() {
        roundKeys.clear();

        string permutedKey;
        permutedKey.reserve(56);
        for (int i = 0; i < 56; ++i) permutedKey += key[pc1[i] - 1];

        string left = permutedKey.substr(0, 28);
        string right = permutedKey.substr(28, 28);

        for (int i = 0; i < 16; ++i) {
            if (i == 0 || i == 1 || i == 8 || i == 15) {
                left = shift_left_once(left);
                right = shift_left_once(right);
            } else {
                left = shift_left_twice(left);
                right = shift_left_twice(right);
}

            string combinedKey = left + right;
            string roundKey;
            roundKey.reserve(48);
            for (int j = 0; j < 48; ++j) roundKey += combinedKey[pc2[j] - 1];
            roundKeys.push_back(roundKey);
        }
    }

    const vector<string>& getRoundKeys() const {
        return roundKeys;
    }
};

class DES {
private:
    const int expansion_table[48] = {
        32,1,2,3,4,5,4,5,
        6,7,8,9,8,9,10,11,
        12,13,12,13,14,15,16,17,
        16,17,18,19,20,21,20,21,
        22,23,24,25,24,25,26,27,
        28,29,28,29,30,31,32,1
    };

    const int permutation_tab[32] = {
        16,7,20,21,29,12,28,17,
        1,15,23,26,5,18,31,10,
        2,8,24,14,32,27,3,9,
        19,13,30,6,22,11,4,25
    };

    const int substitution_boxes[8][4][16] = {{
        14,4,13,1,2,15,11,8,3,10,6,12,5,9,0,7,
        0,15,7,4,14,2,13,1,10,6,12,11,9,5,3,8,
        4,1,14,8,13,6,2,11,15,12,9,7,3,10,5,0,
        15,12,8,2,4,9,1,7,5,11,3,14,10,0,6,13
    }, {
        15,1,8,14,6,11,3,4,9,7,2,13,12,0,5,10,
        3,13,4,7,15,2,8,14,12,0,1,10,6,9,11,5,
        0,14,7,11,10,4,13,1,5,8,12,6,9,3,2,15,
        13,8,10,1,3,15,4,2,11,6,7,12,0,5,14,9
    }, {
        10,0,9,14,6,3,15,5,1,13,12,7,11,4,2,8,
        13,7,0,9,3,4,6,10,2,8,5,14,12,11,15,1,
        13,6,4,9,8,15,3,0,11,1,2,12,5,10,14,7,
        1,10,13,0,6,9,8,7,4,15,14,3,11,5,2,12
    }, {
        7,13,14,3,0,6,9,10,1,2,8,5,11,12,4,15,
        13,8,11,5,6,15,0,3,4,7,2,12,1,10,14,9,
        10,6,9,0,12,11,7,13,15,1,3,14,5,2,8,4,
        3,15,0,6,10,1,13,8,9,4,5,11,12,7,2,14
    }, {
        2,12,4,1,7,10,11,6,8,5,3,15,13,0,14,9,
        14,11,2,12,4,7,13,1,5,0,15,10,3,9,8,6,
        4,2,1,11,10,13,7,8,15,9,12,5,6,3,0,14,
        11,8,12,7,1,14,2,13,6,15,0,9,10,4,5,3
    }, {
        12,1,10,15,9,2,6,8,0,13,3,4,14,7,5,11,
        10,15,4,2,7,12,9,5,6,1,13,14,0,11,3,8,
        9,14,15,5,2,8,12,3,7,0,4,10,1,13,11,6,
        4,3,2,12,9,5,15,10,11,14,1,7,6,0,8,13
    }, {
        4,11,2,14,15,0,8,13,3,12,9,7,5,10,6,1,
        13,0,11,7,4,9,1,10,14,3,5,12,2,15,8,6,
        1,4,11,13,12,3,7,14,10,15,6,8,0,5,9,2,
        6,11,13,8,1,4,10,7,9,5,0,15,14,2,3,12
    }, {
        13,2,8,4,6,15,11,1,10,9,3,14,5,0,12,7,
        1,15,13,8,10,3,7,4,12,5,6,11,0,14,9,2,
        7,11,4,1,9,12,14,2,0,6,10,13,15,3,5,8,
        2,1,14,7,4,10,8,13,15,12,9,0,3,5,6,11
    }};

public:
    string processBlock(const string& input, const vector<string>& round_keys) {
        string perm = initial_permutation(input);
        string left = perm.substr(0, 32);
        string right = perm.substr(32, 32);

        for (int i = 0; i < 16; ++i) {
            string right_expanded;
            right_expanded.reserve(48);
            for (int j = 0; j < 48; ++j) right_expanded += right[expansion_table[j] - 1];

            string xored = Xor(round_keys[i], right_expanded);

            string sbox_result;
            sbox_result.reserve(32);
            for (int j = 0; j < 8; ++j) {
string row_bits = xored.substr(j * 6, 1) + xored.substr(j * 6 + 5, 1);
                int row = convert_binary_to_decimal(row_bits);
                int col = convert_binary_to_decimal(xored.substr(j * 6 + 1, 4));
                int val = substitution_boxes[j][row][col];
                sbox_result += convert_decimal_to_binary(val);
            }

            string perm2;
            perm2.reserve(32);
            for (int j = 0; j < 32; ++j) perm2 += sbox_result[permutation_tab[j] - 1];

            string new_right = Xor(perm2, left);
            left = right;
            right = new_right;
        }

        return inverse_initial_permutation(right + left);
    }
};

bool is_binary_string(const string& s) {
    return !s.empty() && all_of(s.begin(), s.end(), [](char c) { return c == '0' || c == '1'; });
}

vector<string> split_into_blocks_with_zero_padding(const string& input) {
    vector<string> blocks;
    if (input.empty()) return blocks;

    for (size_t i = 0; i < input.size(); i += 64) {
        string block = input.substr(i, 64);
        if (block.size() < 64) block += string(64 - block.size(), '0');
        blocks.push_back(block);
    }
    return blocks;
}

vector<string> get_round_keys(const string& key64) {
    KeyGenerator keygen(key64);
    keygen.generateRoundKeys();
    return keygen.getRoundKeys();
}

string des_encrypt_block(const string& block64, const string& key64) {
    DES des;
    vector<string> keys = get_round_keys(key64);
    return des.processBlock(block64, keys);
}

string des_decrypt_block(const string& block64, const string& key64) {
    DES des;
    vector<string> keys = get_round_keys(key64);
    reverse(keys.begin(), keys.end());
    return des.processBlock(block64, keys);
}

// Helper functions for input processing
string string_to_binary(const string& str) {
    string binary;
    for (char c : str) {
        binary += bitset<8>(c).to_string();
    }
    return binary;
}

string binary_to_string(const string& binary) {
    string str;
    for (size_t i = 0; i < binary.size(); i += 8) {
        string byte = binary.substr(i, 8);
        char c = static_cast<char>(bitset<8>(byte).to_ulong());
        str += c;
    }
    return str;
}

string hex_to_binary(const string& hex) {
    string binary;
    for (char c : hex) {
        int val = isdigit(c) ? c - '0' : 10 + (tolower(c) - 'a');
        binary += bitset<4>(val).to_string();
    }
    return binary;
}

string binary_to_hex(const string& binary) {
    string hex;
    for (size_t i = 0; i < binary.size(); i += 4) {
        string nibble = binary.substr(i, 4);
        int val = bitset<4>(nibble).to_ulong();
        hex += (val < 10) ? ('0' + val) : ('A' + val - 10);
    }
    return hex;
}

string pad_binary(const string& binary) {
    size_t len = binary.size();
    size_t remainder = len % 64;
    if (remainder == 0) return binary;
    size_t padding_needed = 64 - remainder;
    return binary + string(padding_needed, '0');
}

string unpad_binary(const string& binary) {
    size_t last_one = binary.find_last_of('1');
    if (last_one == string::npos) return "";
    return binary.substr(0, last_one + 1);
}

int main(int argc, char* argv[]) {
    if (argc == 1) {
        int mode;
        if (!(cin >> mode)) {
            cerr << "Invalid mode input" << endl;
            return 1;
        }

        if (mode == 1) {
            string plaintext, key;
            cin >> plaintext >> key;
            if (!is_binary_string(plaintext) || !is_binary_string(key) || key.size() != 64) {
                cerr << "Invalid DES encrypt input" << endl;
                return 1;
            }

            vector<string> blocks = split_into_blocks_with_zero_padding(plaintext);
            string ciphertext;
            for (const auto& block : blocks) ciphertext += des_encrypt_block(block, key);
            cout << "Ciphertext: " << ciphertext << '\n';
            return 0;
        }

        if (mode == 2) {
            string ciphertext, key;
            cin >> ciphertext >> key;
            if (!is_binary_string(ciphertext) || !is_binary_string(key) || key.size() != 64 || ciphertext.size() % 64 != 0) {
                cerr << "Invalid DES decrypt input" << endl;
                return 1;
            }

            string plaintext;
            for (size_t i = 0; i < ciphertext.size(); i += 64) {
                plaintext += des_decrypt_block(ciphertext.substr(i, 64), key);
            }
            cout << "Plaintext: " << plaintext << '\n';
            return 0;
        }

        if (mode == 3) {
            string plaintext64, k1, k2, k3;
            cin >> plaintext64 >> k1 >> k2 >> k3;
            if (!is_binary_string(plaintext64) || plaintext64.size() != 64 ||
                !is_binary_string(k1) || k1.size() != 64 ||
                !is_binary_string(k2) || k2.size() != 64 ||
                !is_binary_string(k3) || k3.size() != 64) {
                cerr << "Invalid TripleDES encrypt input" << endl;
                return 1;
            }

            string c1 = des_encrypt_block(plaintext64, k1);
            string c2 = des_decrypt_block(c1, k2);
            string c3 = des_encrypt_block(c2, k3);
            cout << "TripleDES Ciphertext: " << c3 << '\n';
            return 0;
        }

        if (mode == 4) {
            string ciphertext64, k1, k2, k3;
            cin >> ciphertext64 >> k1 >> k2 >> k3;
            if (!is_binary_string(ciphertext64) || ciphertext64.size() != 64 ||
                !is_binary_string(k1) || k1.size() != 64 ||
                !is_binary_string(k2) || k2.size() != 64 ||
                !is_binary_string(k3) || k3.size() != 64) {
                cerr << "Invalid TripleDES decrypt input" << endl;
                return 1;
            }

            string p1 = des_decrypt_block(ciphertext64, k3);
            string p2 = des_encrypt_block(p1, k2);
            string p3 = des_decrypt_block(p2, k1);
            cout << "TripleDES Plaintext: " << p3 << '\n';
            return 0;
        }

        cerr << "Mode must be 1, 2, 3 or 4" << endl;
        return 1;
    }

    if (argc < 5) {
        cout << "Usage: " << argv[0] << " <mode> <operation> <key> [key2] [key3] <plaintext>" << endl;
        cout << "Modes: des, tripledes" << endl;
        cout << "Operations: encrypt, decrypt" << endl;
        cout << "Keys: hex (0x...) or 64-bit binary" << endl;
        cout << "Plaintext: hex (0x...), binary (64-bit multiple), or string" << endl;
        return 1;
    }

    string mode = argv[1];
    string operation = argv[2];

    vector<string> keys;
    string plaintext_str;

    if (mode == "des") {
        if (argc != 5) {
            cout << "DES mode requires: mode operation key plaintext" << endl;
            return 1;
        }
        keys.push_back(argv[3]);
        plaintext_str = argv[4];
    } else if (mode == "tripledes") {
        if (argc != 7) {
            cout << "TripleDES mode requires: mode operation key1 key2 key3 plaintext" << endl;
            return 1;
        }
        keys.push_back(argv[3]);
        keys.push_back(argv[4]);
        keys.push_back(argv[5]);
        plaintext_str = argv[6];
    } else {
        cout << "Error: invalid mode" << endl;
        return 1;
    }

    // Process keys
    for (string& key : keys) {
        if (key.substr(0, 2) == "0x") {
            key = hex_to_binary(key.substr(2));
        }
        if (key.size() != 64) {
            cout << "Error: all keys must be 64 bits" << endl;
            return 1;
        }
    }

    // Process plaintext
    string plaintext_bin;
    if (plaintext_str.substr(0, 2) == "0x") {
        plaintext_bin = hex_to_binary(plaintext_str.substr(2));
    } else if (plaintext_str.find_first_not_of("01") == string::npos && (plaintext_str.size() % 64 == 0 || plaintext_str.size() == 64)) {
        plaintext_bin = plaintext_str;
    } else {
        plaintext_bin = string_to_binary(plaintext_str);
    }

    plaintext_bin = pad_binary(plaintext_bin);

    // Process in 64-bit blocks
    vector<string> blocks;
    for (size_t i = 0; i < plaintext_bin.size(); i += 64) {
        blocks.push_back(plaintext_bin.substr(i, 64));
    }

    string result_bin;

    if (mode == "des") {
        for (const string& block : blocks) {
            if (operation == "encrypt") {
                result_bin += des_encrypt_block(block, keys[0]);
            } else if (operation == "decrypt") {
                result_bin += des_decrypt_block(block, keys[0]);
            } else {
                cout << "Error: invalid operation" << endl;
                return 1;
            }
        }
    } else if (mode == "tripledes") {
        for (const string& block : blocks) {
            if (operation == "encrypt") {
                string temp = des_encrypt_block(block, keys[0]);
                temp = des_decrypt_block(temp, keys[1]);
                result_bin += des_encrypt_block(temp, keys[2]);
            } else if (operation == "decrypt") {
                string temp = des_decrypt_block(block, keys[2]);
                temp = des_encrypt_block(temp, keys[1]);
                result_bin += des_decrypt_block(temp, keys[0]);
            } else {
                cout << "Error: invalid operation" << endl;
                return 1;
            }
        }
    }

    // Output
    if (plaintext_str.substr(0, 2) == "0x" || (plaintext_str.find_first_not_of("01") == string::npos && plaintext_str.size() == 64)) {
        string result_hex = binary_to_hex(result_bin);
        cout << "Result (hex): 0x" << result_hex << endl;
    } else {
        string result_str = binary_to_string(result_bin);
        if (operation == "decrypt") {
            result_bin = unpad_binary(result_bin);
            result_str = binary_to_string(result_bin);
        }
        cout << "Result: " << result_str << endl;
    }

    return 0;
}