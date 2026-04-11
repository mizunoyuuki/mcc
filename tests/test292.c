// EXPECT: 100
// SECTION: シャドーイング（値の独立性）

int main(){
    int a = 100;
    if (0) {
        int a = 1;
        a = a + 10;
    } else {
        int a = 2;
        a = a + 20;
    }
    return a;
}

