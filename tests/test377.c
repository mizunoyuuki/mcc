// EXPECT: 5
// SECTION: typedef struct (エイリアスのポインタでアクセス)
struct Node { int val; };
typedef struct Node Node;
int main(){ Node n; n.val = 5; Node *p; p = &n; return p->val; }
