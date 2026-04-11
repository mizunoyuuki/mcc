// EXPECT: 9
// SECTION: シャドーイング（値の独立性）

int main(){
    int a = 1;
    int r = 0;
    if (1) {
        int a = 2;
        r = r + a;
        if (1) {
            int a = 4;
            r = r + a;
        }
        r = r + a;
    }
    r = r + a;
    return r;
}

