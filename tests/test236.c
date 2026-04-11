// EXPECT: 101
// SECTION: 文字列リテラル

int f(char *s){ return *(s + 1); }
int main(){
    return f("hello");
}

