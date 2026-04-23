#include <openssl/evp.h>
#include <openssl/err.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

void handle_errors() {
    ERR_print_errors_fp(stderr);
    exit(1);
}

int main(int argc, char *argv[]) {
    // 1. 引数のチェック
    if (argc < 2) {
        printf("使用法: %s \"暗号化したいテキスト\"\n", argv[0]);
        return 1;
    }

    // 引数で受け取ったテキスト
    unsigned char *plaintext = (unsigned char *)argv[1];
    
    // 固定の鍵とIV (32バイトと16バイト)
    unsigned char *key = (unsigned char *)"01234567890123456789012345678901"; 
    unsigned char *iv = (unsigned char *)"0123456789012345";

    // 出力バッファ（引数の長さに応じて十分なサイズを確保）
    int plaintext_len = strlen((char *)plaintext);
    unsigned char *ciphertext = malloc(plaintext_len + 32); // パディング分多めに
    unsigned char *decryptedtext = malloc(plaintext_len + 32);
    
    int len, ciphertext_len, decryptedtext_len;

    printf("入力されたテキスト: %s\n", plaintext);

    // --- 暗号化 ---
    EVP_CIPHER_CTX *en_ctx = EVP_CIPHER_CTX_new();
    EVP_EncryptInit_ex(en_ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_EncryptUpdate(en_ctx, ciphertext, &len, plaintext, plaintext_len);
    ciphertext_len = len;
    EVP_EncryptFinal_ex(en_ctx, ciphertext + len, &len);
    ciphertext_len += len;
    EVP_CIPHER_CTX_free(en_ctx);

    printf("暗号化結果（16進数）: ");
    for(int i = 0; i < ciphertext_len; i++) printf("%02x", ciphertext[i]);
    printf("\n");

    // --- 復号 ---
    EVP_CIPHER_CTX *de_ctx = EVP_CIPHER_CTX_new();
    EVP_DecryptInit_ex(de_ctx, EVP_aes_256_cbc(), NULL, key, iv);
    EVP_DecryptUpdate(de_ctx, decryptedtext, &len, ciphertext, ciphertext_len);
    decryptedtext_len = len;
    EVP_DecryptFinal_ex(de_ctx, decryptedtext + len, &len);
    decryptedtext_len += len;
    EVP_CIPHER_CTX_free(de_ctx);

    decryptedtext[decryptedtext_len] = '\0';
    printf("復号結果: %s\n", decryptedtext);

    // メモリ解放
    free(ciphertext);
    free(decryptedtext);

    return 0;
}
