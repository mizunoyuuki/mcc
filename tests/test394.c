// EXPECT: 1
// SECTION: && 短絡評価の確認 (左辺falseなら右辺の関数は呼ばれない)
int g;
int set(){ g = 1; return 1; }
int main(){ g = 0; 0 && set(); return g == 0; }
