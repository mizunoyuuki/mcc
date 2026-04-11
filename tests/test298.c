// EXPECT: 10
// SECTION: シャドーイング（値の独立性）

int main(){
    int a = 0;
    {
        a = 10;
    }
    return a;
}

