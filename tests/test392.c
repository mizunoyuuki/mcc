// EXPECT: 1
// SECTION: && と || の混合 (1 && 0 || 1 → 1)
int main(){ return 1 && 0 || 1; }
