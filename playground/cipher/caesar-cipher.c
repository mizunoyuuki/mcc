#include <stdio.h>
#include <string.h>
#include <ctype.h>

// シーザー暗号: アルファベットをshift文字分ずらす
// 例: shift=3なら A→D, B→E, Z→C

char encrypt_char(char c, int shift) {
    if (!isalpha(c)) return c;
    char base = islower(c) ? 'a' : 'A';
    return (char)((c - base + shift) % 26 + base);
}

char decrypt_char(char c, int shift) {
    return encrypt_char(c, 26 - shift);
}

void encrypt(const char *in, char *out, int shift) {
    for (int i = 0; in[i]; i++)
        out[i] = encrypt_char(in[i], shift);
    out[strlen(in)] = '\0';
}

void decrypt(const char *in, char *out, int shift) {
    for (int i = 0; in[i]; i++)
        out[i] = decrypt_char(in[i], shift);
    out[strlen(in)] = '\0';
}

// 頻度分析: 最も多いアルファベットが 'e' と仮定してshiftを推定
int guess_shift(const char *cipher) {
    int freq[26] = {0};
    int total = 0;
    for (int i = 0; cipher[i]; i++) {
        if (isalpha(cipher[i])) {
            freq[tolower(cipher[i]) - 'a']++;
            total++;
        }
    }
    int max_idx = 0;
    for (int i = 1; i < 26; i++)
        if (freq[i] > freq[max_idx]) max_idx = i;
    // 'e' は4番目(index=4)
    return (max_idx - 4 + 26) % 26;
}

int main() {
    const char *plain = "Hello World";
    int shift = 13; // ROT13

    char cipher[256];
    char decoded[256];

    encrypt(plain, cipher, shift);
    decrypt(cipher, decoded, shift);

    printf("plain:   %s\n", plain);
    printf("shift:   %d\n", shift);
    printf("cipher:  %s\n", cipher);
    printf("decoded: %s\n", decoded);

    // 頻度分析でshiftを推定
    const char *long_cipher = "Uryyb Jbeyq. Guvf vf n grfg bs EBG13 pvcure.";
    int guessed = guess_shift(long_cipher);
    printf("\nfrequency analysis guess: shift=%d\n", guessed);

    char freq_decoded[256];
    decrypt(long_cipher, freq_decoded, guessed);
    printf("freq decoded: %s\n", freq_decoded);

    return 0;
}
