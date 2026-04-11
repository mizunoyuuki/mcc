// EXPECT: 20
// SECTION: グローバル変数とローカル変数の名前衝突

int shared;
int scope_inc_shared(){
    shared = shared + 1;
    return shared;
}
int scope_local_only(){
    int shared = 100;
    return shared;
}
int main(){
    shared = 0;
    scope_inc_shared();
    scope_inc_shared();
    scope_inc_shared();
    scope_local_only();
    return shared + 17;
}

