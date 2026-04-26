// EXPECT: 42
// SECTION: typedef struct (グローバル変数にエイリアスを使う)
struct Box { int v; };
typedef struct Box Box;
Box g;
int main(){ g.v = 42; return g.v; }
