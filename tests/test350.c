// EXPECT: 25
// SECTION: ドット演算子で書いてアロー演算子で読む
struct A {
    int val;
};
int main(){
    struct A a;
    struct A *p;
    p = &a;
    a.val = 25;
    return p->val;
}
