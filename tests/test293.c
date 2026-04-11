// EXPECT: 13
// SECTION: シャドーイング（値の独立性）

int main(){
    int a = 1;
    int b = 2;
    if (1) {
        int a = 10;
        b = b + a;
    }
    return a + b;
}

