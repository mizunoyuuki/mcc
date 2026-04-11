// EXPECT: 104
// SECTION: 文字列リテラル

int f(char *s){ return *s; }
int main(){
    return f("hello");
}

