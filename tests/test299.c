// EXPECT: 15
// SECTION: シャドーイング（値の独立性）

int main(){
    int a = 10;
    int b = 0;
    if (1) {
        int a = 5;
        b = a;
    }
    return a + b;
}

