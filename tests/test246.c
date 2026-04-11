// EXPECT: 5
// SECTION: 文字列リテラル

// コメントアウトの追加
int strlen_impl(char *s){
    int n = 0;
    while (*(s + n) != 0)
        n = n + 1;
    return n;
}
/* コメントアウトの追加
改行
*/
int main(){
    return strlen_impl("hello");
}

