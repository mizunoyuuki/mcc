// EXPECT: 30
// SECTION: 構造体の複数メンバへの代入と加算
struct Point {
    int x;
    int y;
};
int main(){
    struct Point p;
    p.x = 10;
    p.y = 20;
    return p.x + p.y;
}
