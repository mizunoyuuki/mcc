// EXPECT: 7
// SECTION: typedef struct (基本的なエイリアスとメンバアクセス)
struct Point { int x; int y; };
typedef struct Point Point;
int main(){ Point p; p.x = 3; p.y = 4; return p.x + p.y; }
