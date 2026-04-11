// EXPECT: 3
// SECTION: シャドーイング（値の独立性）

int main(){
    int sum = 0;
    sum = sum + 1;
    if (1) {
        int sum = 10;
        sum = sum + 1;
    }
    sum = sum + 2;
    return sum;
}

