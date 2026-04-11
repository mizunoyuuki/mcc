// EXPECT: 12
// SECTION: シャドーイング（値の独立性）

int main(){
    int r = 0;
    {
        int v = 4;
        r = r + v;
    }
    {
        int v = 8;
        r = r + v;
    }
    return r;
}

