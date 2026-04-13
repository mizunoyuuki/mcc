// EXPECT: 10
// SECTION: アロー演算子で書いてドット演算子で読む
struct A {
    int val;
};
int main(){
    struct A a;
    struct A *p;
    p = &a;
    p->val = 10;
    return a.val;
}
