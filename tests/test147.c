// EXPECT: 15
// SECTION: グローバル変数

int x;
int y;
int set_x(){ x = 5; return 0; }
int set_y(){ y = 10; return 0; }
int main(){
    set_x();
    set_y();
    return x + y;
}

