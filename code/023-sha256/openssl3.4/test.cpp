// openssl 3.0
#include <iostream>
#include <stdio.h>
#include <cstring>
#include <openssl/evp.h>
#include <openssl/sha.h>

void sha256(const std::string& input, std::string& output) {
    unsigned char digest[SHA256_DIGEST_LENGTH];
    EVP_MD_CTX* mdctx = EVP_MD_CTX_new();
    EVP_DigestInit(mdctx, EVP_sha256());
    EVP_DigestUpdate(mdctx, input.c_str(), strlen(input.c_str()));
    EVP_DigestFinal(mdctx, digest, NULL);
    EVP_MD_CTX_free(mdctx);
 
    char buf[2 * SHA256_DIGEST_LENGTH + 1];
    buf[2 * SHA256_DIGEST_LENGTH] = '\0';
    for (int i = 0; i < SHA256_DIGEST_LENGTH; i++) {
        sprintf(buf + i * 2, "%02x", digest[i]);
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
