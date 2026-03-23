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
assert 0 'main(){0;}'
assert 42 'main(){42;}'
assert 42 'main(){21+21-10+10;}'
assert 47 'main(){5+6*7;}'
assert 15 'main(){5*(9-6);}'
assert 4 'main(){(3+5)/2;}'
assert 4 'main(){-15+(-3+3)+19;}'
assert 32 'main(){+4*+8;}'

echo "=== 比較演算子 ==="
assert 0 'main(){0==1;}'
assert 1 'main(){12==12;}'
assert 14 'main(){(12==12)+13;}'
assert 1 'main(){0<9;}'
assert 0 'main(){12<10;}'
assert 0 'main(){12>12;}'
assert 1 'main(){12>=12;}'
assert 0 'main(){12   <  11;}'

echo "=== 複数文・ローカル変数 ==="
assert 2 'main(){12-12;13-11;}'
assert 11 'main(){a=12;a-1;}'
assert 12 'main(){a=15;b=a-3;b;}'
assert 10 'main(){a = 11; b = 1; a-b;}'
assert 10 'main(){abc = 23; def = 10; def = 13; abc - def;}'
assert 23 'main(){abc = 23; def = 10; def = 13; abc;}'
assert 13 'main(){abc = 23; def = 10; def = 13; def;}'
assert 14 'main(){
abc = 23;
def = 10;
def = 234 - 220;
}'

echo "=== return ==="
assert 19 'main(){
abc = 19;
name = 111;
return abc;
return name;
}'

echo "=== if/else ==="
assert 11 'main(){
a = 10;
b = 0;
if (a == b)
	return b+3;
else if (a != b + 10)
	return a;
else
	return 11;
}'

echo "=== while ==="
assert 3 'main(){
a = 0;
b = 1;
while (a <= 2)
	a = a+1;
a;
}'

echo "=== for ==="
assert 8 'main(){
i=0;
a=0;
for ( i=0; i<4; i= i+1)
	a = a + 2;
a;
}'

assert 40 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
a;
}'

assert 44 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
	a = a+10;
	b = b+11;
}
b;
}'

echo "=== for + if/else ==="
assert 40 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
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
assert 48 'main(){
a=0;
b=0;
for (i=0; i<4; i= i+1){
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
assert 33 'main(){foo();}'
assert 5 'main(){bar();}'
assert 10 'main(){add_ten();}'
assert 38 'main(){bar() + foo();}'
assert 15 'main(){a = add_ten(); a + bar();}'
assert 36 'main(){f_with_arg(3, 33);}'

echo "=== 関数定義 ==="
assert 46 '
mizuno(yu, ki){
    return 2*23;
}

main(){
	c = mizuno(yu,ki);
	c;
}
'

echo "=== 関数定義（複合テスト） ==="
assert 100 '
compute(){
    a = 0;
    for (i = 0; i < 10; i = i + 1)
        a = a + 10;
    return a;
}

main(){
    return compute();
}
'

assert 1 '
check(x, y){
    if (1 == 1)
        return 1;
    else
        return 0;
}

main(){
    return check(10, 20);
}
'

assert 55 '
sum_to_ten(){
    s = 0;
    i = 1;
    while (i <= 10){
        s = s + i;
        i = i + 1;
    }
    return s;
}

main(){
    return sum_to_ten();
}
'

echo "=== 関数定義（引数を使用） ==="
assert 42 '
id(x){ return x; }
main(){ return id(42); }
'

assert 8 '
add(a, b){ return a + b; }
main(){ return add(3, 5); }
'

assert 5 '
sub(a, b){ return a - b; }
main(){ return sub(10, 5); }
'

assert 15 '
sum_abc(a, b, c){ return a + b + c; }
main(){ return sum_abc(3, 5, 7); }
'

assert 20 '
double_it(x){
    y = x + x;
    return y;
}
main(){ return double_it(10); }
'

assert 55 '
fib(n){
    if (n <= 1) return n;
    return fib(n - 1) + fib(n - 2);
}
main(){ return fib(10); }
'

echo "=== 単項演算子（&, *） ==="

# 基本: &で取ったアドレスを*で即デリファレンス
assert 3 '
main(){
    a = 3;
    return *(&a);
}
'

# ポインタ変数経由で読み取り
assert 42 '
main(){
    a = 42;
    p = &a;
    return *p;
}
'

# 複数変数がある場合に正しいアドレスを参照できるか
assert 20 '
main(){
    a = 10;
    b = 20;
    return *(&b);
}
'

# デリファレンスした値を式の中で使う
assert 30 '
main(){
    a = 10;
    b = 20;
    return *(&a) + *(&b);
}
'

# ポインタ算術で隣の変数を参照
# a: rbp-8, b: rbp-16 → &b + 8 = &a
assert 5 '
main(){
    a = 5;
    b = 7;
    c = &b;
    z = c + 8;
    return *z;
}
'

# 二重デリファレンス **
assert 3 '
main(){
    a = 3;
    b = &a;
    c = &b;
    return **c;
}
'

# 関数引数に対する & と *
assert 77 '
f(x){
    return *(&x);
}
main(){
    return f(77);
}
'

assert 4 '
main(){
	a = 10;
	p = &a;
	*p = 4;
	return a;
}
'

assert 99 '
    main(){
     a = 1;
     p = &a;
     *p = 99;
     return a;
 }
'

echo OK
