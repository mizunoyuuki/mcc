// EXPECT: 21
// SECTION: シャドーイング（値の独立性）

int main(){
    int x = 1;
    int r = 0;
    {
        int x = 2;
        r = r + x;
        {
            int x = 4;
            r = r + x;
            {
                int x = 8;
                r = r + x;
            }
            r = r + x;
        }
        r = r + x;
    }
    r = r + x;
    return r;
}

