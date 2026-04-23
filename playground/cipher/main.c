#include<stdio.h>

int main(){
    int a = 10;
    int b = 20;
    a ^= b;
    b ^= a;
    a ^= b;
    printf("a: %d b: %d", a, b);
}
