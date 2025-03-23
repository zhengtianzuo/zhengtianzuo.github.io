// openssl 1.1
#include <iostream>
#include <openssl/sha.h>

void sha256(const std::string& input, std::string& output) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256_CTX sha256;
    SHA256_Init(&sha256);
    SHA256_Update(&sha256, input.c_str(), input.length());
    SHA256_Final(hash, &sha256);

    char buf[2 * SHA256_DIGEST_LENGTH + 1];
    buf[2 * SHA256_DIGEST_LENGTH] = '\0';
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(buf + i * 2, "%02x", hash[i]);
    }

    output = buf;
}

int main() {
    std::string input = "人生促一事 便致他极";
    std::string output = "";

    sha256(input, output);

    std::cout << "Input: " << input << std::endl;
    std::cout << "SHA-256 Hash: " << output << std::endl;

    return 0;
}
