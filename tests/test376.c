// EXPECT: 10
// SECTION: typedef struct (エイリアスを関数引数に使う)
struct Vec { int a; int b; };
typedef struct Vec Vec;
int sum(Vec *v){ return v->a + v->b; }
int main(){ Vec v; v.a = 6; v.b = 4; return sum(&v); }
