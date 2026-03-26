#!/bin/bash
assert(){
	expected="$1"
	input="$2"

	./mcc "$input" > tmp.s
	cc -o tmp tmp.s test_func.c
	./tmp
	actual="$?"

	if [ "$actual" = "$expected" ]; then
		echo "$input => $actual"
	else
		echo "$input => $expected expexted, but got $actual"
		exit 1
	fi
	echo "****************"
}

echo "=== 数値・四則演算 ==="
assert 0 'int main(){0;}'
assert 42 'int main(){42;}'
assert 42 'int main(){21+21-10+10;}'
assert 47 'int main(){5+6*7;}'
assert 15 'int main(){5*(9-6);}'
assert 4 'int main(){(3+5)/2;}'
assert 4 'int main(){-15+(-3+3)+19;}'
assert 32 'int main(){+4*+8;}'

echo "=== 比較演算子 ==="
assert 0 'int main(){0==1;}'
assert 1 'int main(){12==12;}'
assert 14 'int main(){(12==12)+13;}'
assert 1 'int main(){0<9;}'
assert 0 'int main(){12<10;}'
assert 0 'int main(){12>12;}'
assert 1 'int main(){12>=12;}'
assert 0 'int main(){12   <  11;}'

echo "=== 複数文・ローカル変数 ==="
assert 2 'int main(){12-12;13-11;}'
assert 11 'int main(){int a=12;a-1;}'
assert 12 'int main(){int a=15;int b=a-3;b;}'
assert 10 'int main(){int a = 11; int b = 1; a-b;}'
assert 10 'int main(){int abc = 23; int def = 10; def = 13; abc - def;}'
assert 23 'int main(){int abc = 23; int def = 10; def = 13; abc;}'
assert 13 'int main(){int abc = 23; int def = 10; def = 13; def;}'
assert 14 'int main(){
int abc = 23;
int def = 10;
def = 234 - 220;
}'

echo "=== return ==="
assert 19 'int main(){
int abc = 19;
int name = 111;
return abc;
return name;
}'

echo "=== if/else ==="
assert 11 'int main(){
int a = 10;
int b = 0;
if (a == b)
	return b+3;
else if (a != b + 10)
	return a;
else
	return 11;
}'

echo "=== while ==="
assert 3 'int main(){
int a = 0;
int b = 1;
while (a <= 2)
	a = a+1;
a;
}'

echo "=== for ==="
assert 8 'int main(){
int i=0;
int a=0;
for ( i=0; i<4; i= i+1)
	a = a + 2;
a;
}'

assert 40 'int main(){
int a=0;
int b=0;
for (int i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
a;
}'

assert 44 'int main(){
int a=0;
int b=0;
for (int i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
b;
}'

echo "=== for + if/else ==="
assert 40 'int main(){
int a=0;
int b=0;
for (int i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
	if (a > b){
		a = a + 1;
	} else {
	        b = b + 1;
        }
}

a;
}'
assert 48 'int main(){
int a=0;
int b=0;
for (int i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
	if (a > b){
		a = a + 1;
	} else {
	        b = b + 1;
        }
}

b;
}'

echo "=== 関数呼び出し（外部関数） ==="
assert 33 'int main(){foo();}'
assert 5 'int main(){bar();}'
assert 10 'int main(){add_ten();}'
assert 38 'int main(){bar() + foo();}'
assert 15 'int main(){int a = add_ten(); a + bar();}'
assert 36 'int main(){f_with_arg(3, 33);}'

echo "=== 関数定義 ==="
assert 46 '
int mizuno(yu, ki){
    return 2*23;
}

int main(){
	int yu = 1;
	int ki = 2;
	int c = mizuno(yu,ki);
	c;
}
'

echo "=== 関数定義（複合テスト） ==="
assert 100 '
int compute(){
    int a = 0;
    for (int i = 0; i < 10; i = i + 1)
        a = a + 10;
    return a;
}

int main(){
    return compute();
}
'

assert 1 '
int check(x, y){
    if (1 == 1)
        return 1;
    else
        return 0;
}

int main(){
    return check(10, 20);
}
'

assert 55 '
int sum_to_ten(){
    int s = 0;
    int i = 1;
    while (i <= 10){
        s = s + i;
        i = i + 1;
    }
    return s;
}

int main(){
    return sum_to_ten();
}
'

echo "=== 関数定義（引数を使用） ==="
assert 42 '
int id(x){ return x; }
int main(){ return id(42); }
'

assert 8 '
int add(a, b){ return a + b; }
int main(){ return add(3, 5); }
'

assert 5 '
int sub(a, b){ return a - b; }
int main(){ return sub(10, 5); }
'

assert 15 '
int sum_abc(a, b, c){ return a + b + c; }
int main(){ return sum_abc(3, 5, 7); }
'

assert 20 '
int double_it(x){
    int y = x + x;
    return y;
}
int main(){ return double_it(10); }
'

assert 55 '
int fib(n){
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
int main(){ return fib(10); }
'

echo "=== 単項演算子（&, *） ==="

# 基本: &で取ったアドレスを*で即デリファレンス
assert 3 '
int main(){
    int a = 3;
    return *(&a);
}
'

# ポインタ変数経由で読み取り
assert 42 '
int main(){
    int a = 42;
    int p = &a;
    return *p;
}
'

# 複数変数がある場合に正しいアドレスを参照できるか
assert 20 '
int main(){
    int a = 10;
    int b = 20;
    return *(&b);
}
'

# デリファレンスした値を式の中で使う
assert 30 '
int main(){
    int a = 10;
    int b = 20;
    return *(&a) + *(&b);
}
'

# ポインタ算術で隣の変数を参照
# a: rbp-8, b: rbp-16 → &b + 8 = &a
assert 5 '
int main(){
    int a = 5;
    int b = 7;
    int c = &b;
    int z = c + 8;
    return *z;
}
'

# 二重デリファレンス **
assert 3 '
int main(){
    int a = 3;
    int b = &a;
    int c = &b;
    return **c;
}
'

# 関数引数に対する & と *
assert 77 '
int f(x){
    return *(&x);
}
int main(){
    return f(77);
}
'

assert 4 '
int main(){
	int a = 10;
	int p = &a;
	*p = 4;
	return a;
}
'

assert 99 '
int main(){
     int a = 1;
     int p = &a;
     *p = 99;
     return a;
 }
'

assert 1 '
int main(){
	int a = 1;
	return a;
}
'

assert 10 '
int main(){
	int a;
	a = 10;
	return a;
}
'

echo "=== ポインタ型変数 ==="

# int *p で宣言して初期化、デリファレンスで値を読む
assert 3 '
int main(){
    int a = 3;
    int *p = &a;
    return *p;
}
'

# int *p を宣言後に代入
assert 42 '
int main(){
    int a = 42;
    int *p;
    p = &a;
    return *p;
}
'

# int *p 経由で書き込み
assert 99 '
int main(){
    int a = 1;
    int *p = &a;
    *p = 99;
    return a;
}
'

# ポインタ変数の値そのものを読む（アドレス値の比較はできないが、コピーして使えるか）
assert 7 '
int main(){
    int a = 7;
    int *p = &a;
    int *q = p;
    return *q;
}
'

# int **pp で二重ポインタ
assert 5 '
int main(){
    int a = 5;
    int *p = &a;
    int **pp = &p;
    return **pp;
}
'

# int **pp 経由で書き込み
assert 88 '
int main(){
    int a = 0;
    int *p = &a;
    int **pp = &p;
    **pp = 88;
    return a;
}
'

# ポインタ型変数をデリファレンスして式の中で使う
assert 30 '
int main(){
    int a = 10;
    int b = 20;
    int *pa = &a;
    int *pb = &b;
    return *pa + *pb;
}
'

# ポインタ型変数と通常変数の混在
assert 15 '
int main(){
    int a = 10;
    int *p = &a;
    int b = *p + 5;
    return b;
}
'

echo "=== ポインタ型変数（初期化なし宣言） ==="

# int **pp; の宣言だけでクラッシュしないこと
assert 0 '
int main(){
    int **pp;
    return 0;
}
'

# int *p; の宣言だけでクラッシュしないこと
assert 0 '
int main(){
    int *p;
    return 0;
}
'

# int **pp; を宣言後に代入して二重デリファレンス
assert 5 '
int main(){
    int a = 5;
    int *p = &a;
    int **pp;
    pp = &p;
    return **pp;
}
'

assert 3 '
int main(){
    int x;
    int *y;
    y = &x;
    *y = 3;
    return x;
}
'
