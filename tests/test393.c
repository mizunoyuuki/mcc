// EXPECT: 1
// SECTION: || 短絡評価の確認 (左辺trueなら右辺の関数は呼ばれない)
int g;
int set(){ g = 1; return 1; }
int main(){ g = 0; 1 || set(); return g == 0; }
