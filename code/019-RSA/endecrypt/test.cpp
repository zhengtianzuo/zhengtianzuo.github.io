#include <iostream>
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <fstream>
#include <vector>

//生成2048位私钥 
//openssl genrsa -out private.pem  2048

//导出公钥 
//openssl rsa -in private.pem  -pubout -out public.pem

// 函数：读取私钥文件并加载到RSA结构
RSA* loadPrivateKey(const std::string& filename) {
    // 打开文件
    std::ifstream file(filename, std::ifstream::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    // 将文件内容读取到内存
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // 使用OpenSSL的PEM读取函数加载私钥
    BIO* mem_bio = BIO_new_mem_buf(contents.data(), contents.size());
    if (!mem_bio) {
        std::cerr << "Error: Could not create BIO for memory buffer." << std::endl;
        return nullptr;
    }

    RSA* rsa = PEM_read_bio_RSAPrivateKey(mem_bio, nullptr, nullptr, nullptr);
    BIO_free(mem_bio);

    if (!rsa) {
        std::cerr << "Error: Could not read RSA private key from file." << std::endl;
        ERR_print_errors_fp(stderr);
    }

    return rsa;
}

// 函数：读取公钥文件并加载到RSA结构
RSA* loadPublicKey(const std::string& filename) {
    // 打开文件
    std::ifstream file(filename, std::ifstream::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << std::endl;
        return nullptr;
    }

    // 将文件内容读取到内存
    std::string contents((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());

    // 使用OpenSSL的PEM读取函数加载公钥
    BIO* mem_bio = BIO_new_mem_buf(contents.data(), contents.size());
    if (!mem_bio) {
        std::cerr << "Error: Could not create BIO for memory buffer." << std::endl;
        return nullptr;
    }

    RSA* rsa = PEM_read_bio_RSA_PUBKEY(mem_bio, nullptr, nullptr, nullptr);
    BIO_free(mem_bio);

    if (!rsa) {
        std::cerr << "Error: Could not read RSA public key from file." << std::endl;
        ERR_print_errors_fp(stderr);
    }

    return rsa;
}

std::string rsa_encrypt(RSA* pub_key, const std::string& plaintext) {
    int rsa_len = RSA_size(pub_key);
    std::vector<unsigned char> ciphertext(rsa_len);
    
    int ret = RSA_public_encrypt(
        plaintext.size(),  
        (const unsigned char*)plaintext.data(), 
        ciphertext.data(), 
        pub_key,
        RSA_PKCS1_OAEP_PADDING // 更安全的填充方式 
    );
    
    return std::string(ciphertext.begin(),  ciphertext.end()); 
}

std::string rsa_decrypt(RSA* priv_key, const std::string& ciphertext) {
    int rsa_len = RSA_size(priv_key);
    std::vector<unsigned char> plaintext(rsa_len);
    
    int ret = RSA_private_decrypt(
        ciphertext.size(), 
        (const unsigned char*)ciphertext.data(), 
        plaintext.data(), 
        priv_key,
        RSA_PKCS1_OAEP_PADDING 
    );
    
    return std::string(plaintext.begin(),  plaintext.end()); 
}

int main() {
    RSA* rsa_public = loadPublicKey("public.pem");
    if (rsa_public == nullptr) return -1;
    std::string src_string = "人生促一事 便致他极";
    std::string encrypt_string = rsa_encrypt(rsa_public, src_string);

    RSA* rsa_private = loadPrivateKey("private.pem");
    if (rsa_private == nullptr) return -1;
    std::string decrypt_string = rsa_decrypt(rsa_private, encrypt_string);
    std::cout << "decrypt_string: " << decrypt_string << std::endl;
    return 0;
}
