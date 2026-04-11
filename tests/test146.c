// EXPECT: 100
// SECTION: グローバル変数

int counter;
int inc_counter(){
    counter = counter + 10;
    return 0;
}
int main(){
    counter = 0;
    for (int i = 0; i < 10; i = i + 1)
        inc_counter();
    return counter;
}

