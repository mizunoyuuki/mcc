#include<stdio.h>
#include<string.h>

int main(){
    char *hello = "Hello";
    char key = 0b11100011;
    int len = strlen(hello);

    char hello_cipher[len + 1];
    int i = 0;
    while (hello[i]){
        hello_cipher[i] = hello[i] ^ key;
        i++;
    }
    hello_cipher[len] = '\0';

    printf("cipher: ");
    for (int j = 0; j < len; j++) printf("%02x ", (unsigned char)hello_cipher[j]);
    printf("\n");

    char hello_cipher_2[len + 1];
    i = 0;
    while (hello_cipher[i]){
        hello_cipher_2[i] = hello_cipher[i] ^ key;
        i++;
    }
    hello_cipher_2[len] = '\0';

    printf("decoded: %s\n", hello_cipher_2);

    return 0;
}
