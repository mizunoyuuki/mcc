// EXPECT: 7
// SECTION: シャドーイング（値の独立性）

int main(){
    int x = 7;
    if (1) {
        int x = 1;
        x = x + 10;
    }
    if (0) {
        int x = 2;
    } else {
        int x = 3;
        x = x + 20;
    }
    return x;
}

