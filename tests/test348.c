// EXPECT: 50
// SECTION: アロー演算子で複数メンバにアクセス
struct Point {
    int x;
    int y;
};
int main(){
    struct Point pt;
    struct Point *p;
    p = &pt;
    p->x = 20;
    p->y = 30;
    return p->x + p->y;
}
