// EXPECT: 42
// SECTION: シャドーイング（値の独立性）

int main(){
    int i = 42;
    int j = 0;
    while (j < 3) {
        int i = 1;
        j = j + i;
    }
    return i;
}

