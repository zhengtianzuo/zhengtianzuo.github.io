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

std::string rsa_sign(RSA* priv_key, const std::string& data) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data.data(),  data.size(),  hash);
    
    std::vector<unsigned char> sig(RSA_size(priv_key));
    unsigned int sig_len;
    
    RSA_sign(NID_sha256, hash, SHA256_DIGEST_LENGTH, sig.data(),  &sig_len, priv_key);
    
    return std::string(sig.begin(),  sig.begin()+sig_len); 
}
 
bool rsa_verify(RSA* pub_key, const std::string& data, const std::string& signature) {
    unsigned char hash[SHA256_DIGEST_LENGTH];
    SHA256((const unsigned char*)data.data(),  data.size(),  hash);
    
    return RSA_verify(NID_sha256, hash, SHA256_DIGEST_LENGTH,
                    (const unsigned char*)signature.data(), 
                    signature.size(),  pub_key) == 1;
}

int main() {
    RSA* rsa_private = loadPrivateKey("private.pem");
    if (rsa_private == nullptr) return -1;
    std::string src_string = "人生促一事 便致他极";
    std::string signature_string = rsa_sign(rsa_private, src_string);

    
    RSA* rsa_public = loadPublicKey("public.pem");
    if (rsa_public == nullptr) return -1;
    std::string other_string = "人生促一事 便致他极";
    bool verify = rsa_verify(rsa_public, other_string, signature_string);
    std::cout << "verify: " << verify << std::endl;
    return 0;
}
