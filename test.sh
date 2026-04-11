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
int mizuno(int yu, int ki){
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
int check(int x, int y){
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
int id(int x){ return x; }
int main(){ return id(42); }
'

assert 8 '
int add(int a, int b){ return a + b; }
int main(){ return add(3, 5); }
'

assert 5 '
int sub(int a, int b){ return a - b; }
int main(){ return sub(10, 5); }
'

assert 15 '
int sum_abc(int a, int b, int c){ return a + b + c; }
int main(){ return sum_abc(3, 5, 7); }
'

assert 20 '
int double_it(int x){
    int y = x + x;
    return y;
}
int main(){ return double_it(10); }
'

assert 55 '
int fib(int n){
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
    int *p = &a;
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
    int *c = &b;
    int *z = c + 1;
    return *z;
}
'

# 二重デリファレンス **
assert 3 '
int main(){
    int a = 3;
    int *b = &a;
    int **c = &b;
    return **c;
}
'

# 関数引数に対する & と *
assert 77 '
int f(int x){
    return *(&x);
}
int main(){
    return f(77);
}
'

assert 4 '
int main(){
	int a = 10;
	int *p = &a;
	*p = 4;
	return a;
}
'

assert 99 '
int main(){
     int a = 1;
     int *p = &a;
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

assert 10 '
int main(){
	int a = 10;
	int b = 8;
	int *p = &b + 1;
	return *p;
}
'

echo "=== 関数引数にポインタ型 ==="

  # 関数引数に int * を使ってデリファレンス
  assert 42 '
  int deref(int *p){ return *p; }
  int main(){
      int a = 42;
      return deref(&a);
  }
  '

  # ポインタ引数経由で書き込み
  assert 99 '
  int set_val(int *p){
      *p = 99;
      return 0;
  }
  int main(){
      int a = 1;
      set_val(&a);
      return a;
  }
  '

  # ポインタ引数 + 通常引数の混在
  assert 30 '
  int add_deref(int *p, int b){ return *p + b; }
  int main(){
      int a = 10;
      return add_deref(&a, 20);
  }
  '

  # ポインタ引数でポインタ加算
  assert 5 '
  int read_next(int *p){ return *(p + 1); }
  int main(){
      int a = 5;
      int b = 7;
      return read_next(&b);
  }
  '

  # 二重ポインタ引数
  assert 3 '
int deref(int **pp){ return **pp; }
int main(){
	int a = 3;
	int *p = &a;
	return deref(&p);
}
'

assert 4 '
int main(){
	int a;
	return sizeof(a);
}
'

assert 8 '
int main(){
	int *p;
	return sizeof(p);
}
'

assert 4 '
int main(){
	int a = 42;
	return sizeof(a);
}
'

assert 8 '
int main(){
	int a;
	return sizeof(a) + sizeof(a);
}
'

assert 4 '
int main(){
	int a;
	int b = sizeof(a);
	return b;
}
'

assert 1 '
int main(){
	int a;
	if (sizeof(a) == 4) return 1;
	return 0;
}
'

assert 10 '
int main(){
	int a;
	a = 10;
	if (sizeof(int) == 4){
		return a;
	} else {
		return 12;
	}
}
'

echo "=== sizeof(ポインタ型) ==="

assert 8 '
int main(){
	return sizeof(int **);
}
'

assert 8 '
int main(){
	return sizeof(char *);
}
'

assert 4 '
int main(){
	return sizeof(1 + 5);
}
'

assert 4 '
int main(){
	int a = 8;
	int b = 12;
	int *c = &b;
	return sizeof(*(c+1));
}
'

assert 4 '
int main(){
	int a = 8;
	int b = 12;
	int *c = &b;
	return sizeof(*c+1);
}
'

assert 8 '
int main(){
	int a = 8;
	int b = 12;
	int *c = &b;
	return sizeof(c+2);
}'


assert 1 '
int main(){
	char a;
	return sizeof(a);
}
'

echo "=== char型変数 ==="

  # charの基本代入と読み取り
  assert 5 '
  int main(){
      char a = 5;
      return a;
  }
  '

  # char同士の演算
  assert 10 '
  int main(){
      char a = 3;
      char b = 7;
      return a + b;
  }
  '

  assert 4 '
  int main(){
      char a = 12;
      char b = 8;
      return a - b;
  }
  '

  assert 6 '
  int main(){
      char a = 2;
      char b = 3;
      return a * b;
  }
  '

  assert 5 '
  int main(){
      char a = 15;
      char b = 3;
      return a / b;
  }
  '

  # charとintの混在演算
  assert 30 '
  int main(){
      char a = 10;
      int b = 20;
      return a + b;
  }
  '

  assert 200 '
  int main(){
      int a = 100;
      char b = 2;
      return a * b;
  }
  '

  # char変数が隣のint変数を壊さないこと
  assert 99 '
  int main(){
      int a = 99;
      char b = 1;
      return a;
  }
  '

  assert 42 '
  int main(){
      char a = 1;
      int b = 42;
      return b;
  }
  '

  # char変数が隣のchar変数を壊さないこと
  assert 3 '
  int main(){
      char a = 3;
      char b = 7;
      return a;
  }
  '

  assert 7 '
  int main(){
      char a = 3;
      char b = 7;
      return b;
  }
  '

  # charの再代入
  assert 20 '
  int main(){
      char a = 10;
      a = 20;
      return a;
  }
  '

  # charをループで使う
  assert 10 '
  int main(){
      char a = 0;
      for (int i = 0; i < 10; i = i + 1)
          a = a + 1;
      return a;
  }
  '

  # charをif条件で使う
  assert 1 '
  int main(){
      char a = 5;
      if (a == 5) return 1;
      return 0;
  }
  '

  # charの比較
  assert 1 '
  int main(){
      char a = 3;
      char b = 5;
      return a < b;
  }
  '

  echo "=== char型ポインタ ==="

  # char *p でデリファレンス
  assert 7 '
  int main(){
      char a = 7;
      char *p = &a;
      return *p;
  }
  '

  # char *p 経由で書き込み
  assert 50 '
  int main(){
      char a = 1;
      char *p = &a;
      *p = 50;
      return a;
  }
  '

  # char *を関数引数に
  assert 33 '
  int deref_char(char *p){ return *p; }
  int main(){
      char a = 33;
      return deref_char(&a);
  }
  '

  echo "=== sizeof(char関連) ==="

  assert 1 '
  int main(){
      return sizeof(char);
  }
  '

  assert 1 '
  int main(){
      char a = 5;
      return sizeof(a);
  }
  '

  assert 8 '
  int main(){
      char *p;
      return sizeof(p);
  }
  '

  assert 8 '
  int main(){
      char **pp;
      return sizeof(pp);
  }
  '

  echo "=== int/char/ptr サイズの混在確認 ==="

  # sizeof が正しく型ごとに異なる値を返す
  assert 1 '
  int main(){
      if (sizeof(char) == 1)
          if (sizeof(int) == 4)
              return 1;
      return 0;
  }
  '

  # 複数の異なる型変数の宣言
  assert 55 '
  int main(){
      int a = 50;
      char b = 5;
      int *p = &a;
      return *p + b;
  }
  '

  # int, char, ポインタが入り混じった関数
  assert 15 '
  int mixed(int a, char b){
      return a + b;
  }
  int main(){
      return mixed(10, 5);
  }
  '

echo "=== 配列の定義 ==="

# 基本: int配列の宣言（クラッシュしないこと）
assert 0 '
int main(){
    int a[5];
    return 0;
}
'

# 基本: char配列の宣言（クラッシュしないこと）
assert 0 '
int main(){
    char a[10];
    return 0;
}
'

# 配列の前後の変数が壊れないこと（前の変数を読む）
assert 5 '
int main(){
    int a = 5;
    int b[10];
    return a;
}
'

# 配列の前後の変数が壊れないこと（後の変数を読む）
assert 10 '
int main(){
    int a = 5;
    int b[10];
    int c = 10;
    return c;
}
'

# 配列の前の変数にポインタ経由でアクセス（オフセット検証）
# a: offset=4, b[10]: offset=44, c: offset=48
# &c + 11 = &c + 11*4bytes = rbp-48 + 44 = rbp-4 = &a
assert 5 '
int main(){
    int a = 5;
    int b[10];
    int c = 10;
    return *(&c+11);
}
'

# 配列だけの宣言で他の変数なし
assert 0 '
int main(){
    int arr[3];
    return 0;
}
'

# char配列の前後の変数が壊れないこと
assert 42 '
int main(){
    int x = 42;
    char buf[20];
    int y = 99;
    return x;
}
'

assert 99 '
int main(){
    int x = 42;
    char buf[20];
    int y = 99;
    return y;
}
'

# 複数の配列を宣言
assert 0 '
int main(){
    int a[5];
    int b[3];
    return 0;
}
'

# 複数の配列と変数の混在
assert 7 '
int main(){
    int x = 7;
    int a[5];
    int y = 13;
    int b[3];
    int z = 21;
    return x;
}
'

assert 13 '
int main(){
    int x = 7;
    int a[5];
    int y = 13;
    int b[3];
    int z = 21;
    return y;
}
'

assert 21 '
int main(){
    int x = 7;
    int a[5];
    int y = 13;
    int b[3];
    int z = 21;
    return z;
}
'

# sizeof で配列の合計サイズが正しいか（int[10] = 40バイト、40>255なので比較で確認）
assert 1 '
int main(){
    int a[10];
    if (sizeof(a) == 40) return 1;
    return 0;
}
'

# sizeof(int[5]) = 20バイト
assert 1 '
int main(){
    int a[5];
    if (sizeof(a) == 20) return 1;
    return 0;
}
'

# sizeof(char[10]) = 10バイト
assert 10 '
int main(){
    char a[10];
    return sizeof(a);
}
'

# sizeof(char[1]) = 1バイト
assert 1 '
int main(){
    char a[1];
    return sizeof(a);
}
'

# sizeof(int[1]) = 4バイト
assert 4 '
int main(){
    int a[1];
    return sizeof(a);
}
'

# 配列宣言がforループ内の変数と干渉しないこと
assert 10 '
int main(){
    int arr[5];
    int sum = 0;
    for (int i = 0; i < 5; i = i + 1)
        sum = sum + 2;
    return sum;
}
'

# 配列宣言が関数引数と干渉しないこと
assert 30 '
int f(int x){
    int buf[10];
    return x;
}
int main(){
    return f(30);
}
'

# 配列宣言 + ポインタ変数の共存
assert 77 '
int main(){
    int a = 77;
    int arr[5];
    int *p = &a;
    return *p;
}
'

# 配列の後に宣言した変数にポインタ経由で書き込み
assert 55 '
int main(){
    int arr[3];
    int a = 0;
    int *p = &a;
    *p = 55;
    return a;
}
'
  assert 1 '
  int main(){
	  char a = 10;
	  char b = 10;
	  return sizeof(a + b);
  }
  '

echo "=== 配列からポインタへの暗黙変換 ==="

# 基本: *a で先頭要素に書き込み・読み取り
assert 1 '
int main(){
    int a[2];
    *a = 1;
    return *a;
}
'

# *(a+1) で2番目の要素にアクセス
assert 2 '
int main(){
    int a[2];
    *a = 1;
    *(a + 1) = 2;
    return *(a + 1);
}
'

# 配列をポインタ変数に代入して使う（p = a）
assert 3 '
int main(){
    int a[2];
    *a = 1;
    *(a + 1) = 2;
    int *p;
    p = a;
    return *p + *(p + 1);
}
'

# 配列の先頭要素のデリファレンスで書き込み
assert 42 '
int main(){
    int a[3];
    *a = 42;
    *(a + 1) = 10;
    *(a + 2) = 20;
    return *a;
}
'

# 配列の各要素の合計
assert 60 '
int main(){
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    return *a + *(a + 1) + *(a + 2);
}
'

# ポインタ変数にdecayした配列を代入し、ポインタ算術で全要素アクセス
assert 6 '
int main(){
    int a[3];
    *a = 1;
    *(a + 1) = 2;
    *(a + 2) = 3;
    int *p = a;
    return *p + *(p + 1) + *(p + 2);
}
'

# decayしてもsizeofは配列全体のサイズを返すこと
assert 1 '
int main(){
    int a[5];
    if (sizeof(a) == 20) return 1;
    return 0;
}
'

# 配列の前後の変数を壊さないこと
assert 99 '
int main(){
    int x = 99;
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    return x;
}
'

assert 77 '
int main(){
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    int y = 77;
    return y;
}
'

# 配列の前後の変数両方が無事であること
assert 176 '
int main(){
    int x = 99;
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    int y = 77;
    return x + y;
}
'

# char配列のdecay
assert 3 '
int main(){
    char a[3];
    *a = 1;
    *(a + 1) = 2;
    *(a + 2) = 3;
    char *p = a;
    return *p + *(p + 1);
}
'

# char配列の全要素合計
assert 6 '
int main(){
    char a[3];
    *a = 1;
    *(a + 1) = 2;
    *(a + 2) = 3;
    return *a + *(a + 1) + *(a + 2);
}
'

# 配列をforループで初期化・合計
assert 10 '
int main(){
    int a[5];
    for (int i = 0; i < 5; i = i + 1)
        *(a + i) = 2;
    int sum = 0;
    for (int j = 0; j < 5; j = j + 1)
        sum = sum + *(a + j);
    return sum;
}
'

# 配列をforループでインデックスごとに異なる値を設定
assert 10 '
int main(){
    int a[5];
    for (int i = 0; i < 5; i = i + 1)
        *(a + i) = i;
    return *(a + 0) + *(a + 1) + *(a + 2) + *(a + 3) + *(a + 4);
}
'

# 関数引数として配列をポインタで受け取る
assert 15 '
int sum(int *p){
    return *p + *(p + 1) + *(p + 2);
}
int main(){
    int a[3];
    *a = 3;
    *(a + 1) = 5;
    *(a + 2) = 7;
    return sum(a);
}
'

# 関数内でポインタ経由で配列を書き換え
assert 99 '
int set_first(int *p){
    *p = 99;
    return 0;
}
int main(){
    int a[3];
    *a = 0;
    set_first(a);
    return *a;
}
'

# 複数の配列が独立していること
assert 30 '
int main(){
    int a[2];
    int b[2];
    *a = 10;
    *(a + 1) = 20;
    *b = 100;
    *(b + 1) = 200;
    return *a + *(a + 1);
}
'

assert 30 '
int main(){
    int a[2];
    int b[2];
    *a = 10;
    *(a + 1) = 20;
    *b = 10;
    *(b + 1) = 20;
    return *b + *(b + 1);
}
'

# 配列とポインタ変数の共存
assert 42 '
int main(){
    int a[3];
    *a = 42;
    int *p = a;
    int *q = a;
    return *q;
}
'

# ポインタ変数で配列の途中を指す
assert 20 '
int main(){
    int a[3];
    *a = 10;
    *(a + 1) = 20;
    *(a + 2) = 30;
    int *p = a + 1;
    return *p;
}
'

# 配列を使った条件分岐
assert 1 '
int main(){
    int a[2];
    *a = 10;
    *(a + 1) = 20;
    if (*a == 10) return 1;
    return 0;
}
'

  assert 3 '
  int main(){
      int a[2];
      a[0] = 1;
      a[1] = 2;
      return a[0] + a[1];
  }
  '

assert 2 '
  int main(){
      int a[2];
      a[0] = 0;
      a[1] = 1;
      a[1] = 2;
      return a[0] + a[1];
  }
  '

assert 42 '
  int main(){
      int a[2];
      a[1] = 1;
      a[1] = 2;
      a[1] = 42;
      return a[0] + a[1];
  }
  '


echo "=== グローバル変数 ==="

# 宣言のみ（クラッシュしないこと）
assert 1 '
int a;
int main(){
    return 1;
}
'

# 代入して読み取り
assert 42 '
int a;
int main(){
    a = 42;
    return a;
}
'

# ゼロ初期化されていること
assert 0 '
int a;
int main(){
    return a;
}
'

# 複数のグローバル変数
assert 30 '
int a;
int b;
int main(){
    a = 10;
    b = 20;
    return a + b;
}
'

# グローバル変数同士の演算
assert 6 '
int x;
int y;
int main(){
    x = 2;
    y = 3;
    return x * y;
}
'

# グローバル変数の再代入
assert 99 '
int a;
int main(){
    a = 10;
    a = 99;
    return a;
}
'

# グローバル変数をループで使う
assert 10 '
int a;
int main(){
    a = 0;
    for (int i = 0; i < 10; i = i + 1)
        a = a + 1;
    return a;
}
'

# グローバル変数をif条件で使う
assert 1 '
int a;
int main(){
    a = 5;
    if (a == 5) return 1;
    return 0;
}
'

# グローバル変数をwhile文で使う
assert 5 '
int a;
int main(){
    a = 0;
    while (a < 5)
        a = a + 1;
    return a;
}
'

# グローバル変数とローカル変数の共存
assert 30 '
int g;
int main(){
    g = 10;
    int l = 20;
    return g + l;
}
'

# ローカル変数がグローバル変数を壊さないこと
assert 10 '
int g;
int main(){
    g = 10;
    int a = 99;
    int b = 77;
    return g;
}
'

# 関数間でグローバル変数を共有
assert 42 '
int g;
int set_g(){
    g = 42;
    return 0;
}
int main(){
    set_g();
    return g;
}
'

# 別の関数でグローバル変数を変更して読み取り
assert 100 '
int counter;
int inc_counter(){
    counter = counter + 10;
    return 0;
}
int main(){
    counter = 0;
    for (int i = 0; i < 10; i = i + 1)
        inc_counter();
    return counter;
}
'

# 複数のグローバル変数を複数の関数で使う
assert 15 '
int x;
int y;
int set_x(){ x = 5; return 0; }
int set_y(){ y = 10; return 0; }
int main(){
    set_x();
    set_y();
    return x + y;
}
'

# グローバル変数を関数の戻り値に使う
assert 7 '
int g;
int get_g(){ return g; }
int main(){
    g = 7;
    return get_g();
}
'

# グローバル変数を式の中で使う
assert 25 '
int a;
int b;
int main(){
    a = 10;
    b = 15;
    int c = a + b;
    return c;
}
'

# char型グローバル変数
assert 5 '
char c;
int main(){
    c = 5;
    return c;
}
'

# char型グローバル変数の演算
assert 15 '
char a;
char b;
int main(){
    a = 7;
    b = 8;
    return a + b;
}
'

# int と char のグローバル変数の混在
assert 50 '
int a;
char b;
int main(){
    a = 40;
    b = 10;
    return a + b;
}
'

echo "=== グローバル変数（複合テスト） ==="

# グローバル変数 + sizeof
assert 4 '
int g;
int main(){
    return sizeof(g);
}
'

assert 1 '
char g;
int main(){
    return sizeof(g);
}
'

# sizeof(グローバル変数) を式の中で使う
assert 8 '
int g;
int main(){
    return sizeof(g) + sizeof(g);
}
'

# sizeof(グローバル変数) を条件分岐で使う
assert 1 '
int g;
int main(){
    if (sizeof(g) == 4) return 1;
    return 0;
}
'

# グローバル変数 + ポインタ（アドレス取得・デリファレンス）
assert 42 '
int g;
int main(){
    g = 42;
    int *p = &g;
    return *p;
}
'

# ポインタ経由でグローバル変数に書き込み
assert 99 '
int g;
int main(){
    g = 0;
    int *p = &g;
    *p = 99;
    return g;
}
'

# グローバル変数のアドレスを関数に渡す
assert 77 '
int g;
int write_ptr(int *p){
    *p = 77;
    return 0;
}
int main(){
    g = 0;
    write_ptr(&g);
    return g;
}
'

# グローバル変数のアドレスを関数引数で受け取ってデリファレンス
assert 55 '
int g;
int read_ptr(int *p){
    return *p;
}
int main(){
    g = 55;
    return read_ptr(&g);
}
'

# グローバル変数 + while + ポインタ
assert 50 '
int g;
int main(){
    g = 0;
    int *p = &g;
    while (g < 50)
        *p = *p + 10;
    return g;
}
'

# グローバル変数 + for + ポインタ
assert 20 '
int g;
int main(){
    g = 0;
    int *p = &g;
    for (int i = 0; i < 5; i = i + 1)
        *p = *p + 4;
    return g;
}
'

# グローバル変数 + if + ポインタ
assert 1 '
int g;
int main(){
    g = 42;
    int *p = &g;
    if (*p == 42) return 1;
    return 0;
}
'

# グローバル変数 + 二重ポインタ
assert 10 '
int g;
int main(){
    g = 10;
    int *p = &g;
    int **pp = &p;
    return **pp;
}
'

# 二重ポインタ経由でグローバル変数に書き込み
assert 88 '
int g;
int main(){
    g = 0;
    int *p = &g;
    int **pp = &p;
    **pp = 88;
    return g;
}
'

# char型グローバル変数 + ポインタ
assert 33 '
char g;
int main(){
    g = 33;
    char *p = &g;
    return *p;
}
'

# char型グローバル変数にポインタ経由で書き込み
assert 77 '
char g;
int main(){
    g = 0;
    char *p = &g;
    *p = 77;
    return g;
}
'

# グローバル変数 + ローカル変数 + ポインタの混在
assert 30 '
int g;
int main(){
    g = 10;
    int l = 20;
    int *pg = &g;
    int *pl = &l;
    return *pg + *pl;
}
'

# グローバル変数をループカウンタ的に使う + sizeof で条件分岐
assert 40 '
int g;
int main(){
    g = 0;
    if (sizeof(g) == 4){
        for (int i = 0; i < 10; i = i + 1)
            g = g + 4;
    }
    return g;
}
'

# 複数グローバル変数 + 関数 + ポインタ + while
assert 30 '
int x;
int y;
int swap_via_ptr(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 0;
}
int main(){
    x = 20;
    y = 10;
    swap_via_ptr(&x, &y);
    return x + y;
}
'

# swap後の値が入れ替わっていること
assert 10 '
int x;
int y;
int swap_via_ptr(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 0;
}
int main(){
    x = 20;
    y = 10;
    swap_via_ptr(&x, &y);
    return x;
}
'

assert 20 '
int x;
int y;
int swap_via_ptr(int *a, int *b){
    int tmp = *a;
    *a = *b;
    *b = tmp;
    return 0;
}
int main(){
    x = 20;
    y = 10;
    swap_via_ptr(&x, &y);
    return y;
}
'

# グローバル変数を使った再帰的なカウント
assert 10 '
int g;
int count_up(){
    if (g >= 10) return g;
    g = g + 1;
    return count_up();
}
int main(){
    g = 0;
    return count_up();
}
'

# グローバル変数 + ローカル配列 + ポインタの複合
assert 60 '
int g;
int main(){
    int arr[3];
    *arr = 10;
    *(arr + 1) = 20;
    *(arr + 2) = 30;
    g = *arr + *(arr + 1) + *(arr + 2);
    return g;
}
'

# グローバル変数を条件に使ったwhileループ + ポインタ書き込み
assert 100 '
int g;
int main(){
    g = 0;
    int *p = &g;
    while (*p < 100){
        *p = *p + 5;
    }
    return g;
}
'

  echo "=== ポインタ型グローバル変数 ==="

  # ポインタ型グローバル変数の宣言（クラッシュしないこと）
  assert 0 '
  int *gp;
  int main(){
      return 0;
  }
  '

  # ポインタ型グローバル変数にローカル変数のアドレスを代入して読み取り
  assert 42 '
  int *gp;
  int main(){
      int a = 42;
      gp = &a;
      return *gp;
  }
  '

  # ポインタ型グローバル変数経由で書き込み
  assert 99 '
  int *gp;
  int main(){
      int a = 0;
      gp = &a;
      *gp = 99;
      return a;
  }
  '

  # ポインタ型グローバル変数を関数間で共有
  assert 77 '
  int *gp;
  int set_gp(int *p){
      gp = p;
      return 0;
  }
  int main(){
      int a = 77;
      set_gp(&a);
      return *gp;
  }
  '

  # ポインタ型グローバル変数 + 別のグローバル変数のアドレス
  assert 10 '
  int val;
  int *gp;
  int main(){
      val = 10;
      gp = &val;
      return *gp;
  }
  '

  # ポインタ型グローバル変数経由でグローバル変数に書き込み
  assert 55 '
  int val;
  int *gp;
  int main(){
      val = 0;
      gp = &val;
      *gp = 55;
      return val;
  }
  '

  # 二重ポインタ型グローバル変数
  assert 3 '
  int **gpp;
  int main(){
      int a = 3;
      int *p = &a;
      gpp = &p;
      return **gpp;
  }
  '

  # char型ポインタのグローバル変数
  assert 25 '
  char *gcp;
  int main(){
      char a = 25;
      gcp = &a;
      return *gcp;
  }
  '

  # ポインタ型グローバル変数のsizeof
  assert 8 '
  int *gp;
  int main(){
      return sizeof(gp);
  }
  '

  # ポインタ型グローバル変数を使ったポインタ算術
  assert 20 '
  int *gp;
  int main(){
      int a[3];
      *a = 10;
      *(a + 1) = 20;
      *(a + 2) = 30;
      gp = a + 1;
      return *gp;
  }
  '

  # 複数のポインタ型グローバル変数
  assert 30 '
  int *gpa;
  int *gpb;
  int main(){
      int a = 10;
      int b = 20;
      gpa = &a;
      gpb = &b;
      return *gpa + *gpb;
  }
  '

  echo "=== ポインタ型返り値の関数 ==="

  # int* を返す関数（ローカル変数のアドレスを返す）
  assert 42 '
  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 42;
      return *get_ptr(&a);
  }
  '

  # int* を返す関数で書き込み
  assert 99 '
  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 0;
      *get_ptr(&a) = 99;
      return a;
  }
  '

  # char* を返す関数
  assert 7 '
  char *get_ptr(char *p){ return p; }
  int main(){
      char a = 7;
      return *get_ptr(&a);
  }
  '

  # ポインタ引数をそのまま返す関数
  assert 10 '
  int *identity(int *p){ return p; }
  int main(){
      int a = 10;
      int *p = identity(&a);
      return *p;
  }
  '

  # ポインタ算術して返す関数
  assert 20 '
  int *second(int *p){ return p + 1; }
  int main(){
      int a[2];
      *a = 10;
      *(a + 1) = 20;
      return *second(a);
  }
  '

  # int** を返す関数
  assert 5 '
  int **get_pp(int **pp){ return pp; }
  int main(){
      int a = 5;
      int *p = &a;
      int **pp = get_pp(&p);
      return **pp;
  }
  '

  # ポインタ返り値を変数に格納して使う
  assert 30 '
  int *get_ptr(int *p){ return p; }
  int main(){
      int a = 10;
      int b = 20;
      int *pa = get_ptr(&a);
      int *pb = get_ptr(&b);
      return *pa + *pb;
  }
  '

  # グローバル変数のアドレスをポインタ返り値で返す
  assert 77 '
  int g;
  int *get_gptr(){ return &g; }
  int main(){
      g = 77;
      return *get_gptr();
  }
  '

  # ポインタ返り値の関数でグローバル変数に書き込み
  assert 55 '
  int g;
  int *get_gptr(){ return &g; }
  int main(){
      *get_gptr() = 55;
      return g;
  }
  '

  # ポインタ返り値 + ループ
  assert 15 '
  int *get_ptr(int *p){ return p; }
  int main(){
      int a[3];
      *a = 1;
      *(a + 1) = 2;
      *(a + 2) = 3;
      int sum = 0;
      for (int i = 0; i < 3; i = i + 1)
          sum = sum + *get_ptr(a + i);
      return sum + 9;
  }
  '

# 配列型のグローバル変数
assert 1 '
int a[10];
int main(){
	return 1;
}
'

assert 7 '
int a[10];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	*(a + 3) = 4;
	*(a + 4) = 5;
	*(a + 5) = 6;
	*(a + 6) = 7;
	return *(a + 6);
}
'

# ゼロ初期化されていること
assert 0 '
int a[5];
int main(){
	return *a;
}
'

# 先頭要素の書き込み・読み取り
assert 42 '
int a[5];
int main(){
	*a = 42;
	return *a;
}
'

# 複数要素の書き込み・読み取り
assert 6 '
int a[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	return *a + *(a + 1) + *(a + 2);
}
'

# グローバル配列をポインタ変数に代入して使う
assert 10 '
int a[3];
int main(){
	*a = 10;
	int *p = a;
	return *p;
}
'

# グローバル配列の要素にポインタ経由で書き込み
assert 99 '
int a[3];
int main(){
	*a = 0;
	int *p = a;
	*p = 99;
	return *a;
}
'

# 複数のグローバル配列が独立していること
assert 1 '
int a[3];
int b[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*b = 10;
	*(b + 1) = 20;
	return *a;
}
'

assert 20 '
int a[3];
int b[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*b = 10;
	*(b + 1) = 20;
	return *(b + 1);
}
'

# グローバル配列をforループで使う
assert 10 '
int a[5];
int main(){
	int i;
	for (i = 0; i < 5; i = i + 1)
		*(a + i) = 2;
	int sum = 0;
	for (i = 0; i < 5; i = i + 1)
		sum = sum + *(a + i);
	return sum;
}
'

# 関数をまたいでグローバル配列を使う
assert 7 '
int a[3];
int set_vals(){
	*a = 3;
	*(a + 1) = 4;
	return 0;
}
int main(){
	set_vals();
	return *a + *(a + 1);
}
'

# グローバル配列を関数引数として渡す（ポインタとして）
assert 6 '
int sum(int *p){
	return *p + *(p + 1) + *(p + 2);
}
int a[3];
int main(){
	*a = 1;
	*(a + 1) = 2;
	*(a + 2) = 3;
	return sum(a);
}
'

# char型グローバル配列
assert 0 '
char a[10];
int main(){
	return *a;
}
'

assert 5 '
char a[10];
int main(){
	*a = 5;
	return *a;
}
'

assert 5 '
char s[3];
int main(){
	s[1] = 1;
	s[2] = 5;
	return s[2];
}
'

echo "=== 文字定数 ==="

# 基本的なASCII文字
assert 97  "int main(){ return 'a'; }"
assert 65  "int main(){ return 'A'; }"
assert 122 "int main(){ return 'z'; }"
assert 48  "int main(){ return '0'; }"

# エスケープシーケンス
assert 10  "int main(){ return '\n'; }"
assert 9   "int main(){ return '\t'; }"
assert 13  "int main(){ return '\r'; }"
assert 0   "int main(){ return '\0'; }"
assert 92  "int main(){ return '\\\\'; }"
assert 39  "int main(){ return '\''; }"

# char変数への代入
assert 65  "int main(){ char c = 'A'; return c; }"
assert 97  "int main(){ int x = 'a'; return x; }"

# 算術演算
assert 1   "int main(){ return 'b' - 'a'; }"
assert 25  "int main(){ return 'z' - 'a'; }"
assert 194 "int main(){ return 'a' + 'a'; }"

# 比較演算
assert 1   "int main(){ return 'a' == 97; }"
assert 1   "int main(){ return 'A' < 'a'; }"
assert 0   "int main(){ return 'z' < 'a'; }"

# if条件での使用
assert 1   "int main(){ if ('a' == 97) return 1; return 0; }"
assert 1   "int main(){ char c = 'x'; if (c == 'x') return 1; return 0; }"
assert 0   "int main(){ char c = 'y'; if (c == 'x') return 1; return 0; }"

# 関数引数として渡す
assert 97  "int id(int x){ return x; } int main(){ return id('a'); }"

assert 4 "
int main(){
	return sizeof('A');
}
"

assert 3 "
int main(){
	char x[3];
	x[0] = -1;
	x[1] = 2;
	int y;
	y = 4;
	return x[0] + y;
}
"

assert 5 '
int main(){
	char *s;
	s = "Hello, World!";
	int a;
	a = 5;
	return 5;
}
'

echo "=== 文字列リテラル ==="

# 基本: 代入してクラッシュしないこと
assert 0 '
int main(){
    char *s = "hello";
    return 0;
}
'

# *s で先頭文字を取得 ('h' = 104)
assert 104 '
int main(){
    char *s = "hello";
    return *s;
}
'

# s[0] でインデックスアクセス
assert 104 '
int main(){
    char *s = "hello";
    return s[0];
}
'

# s[1] で2番目の文字 ('e' = 101)
assert 101 '
int main(){
    char *s = "hello";
    return s[1];
}
'

# s[2] で3番目の文字 ('l' = 108)
assert 108 '
int main(){
    char *s = "hello";
    return s[2];
}
'

# *(s+i) でポインタ算術
assert 111 '
int main(){
    char *s = "hello";
    return *(s + 4);
}
'

# 文字列を関数引数に渡す
assert 104 '
int f(char *s){ return *s; }
int main(){
    return f("hello");
}
'

# 関数内でポインタ算術
assert 101 '
int f(char *s){ return *(s + 1); }
int main(){
    return f("hello");
}
'

# 複数の文字列リテラル
assert 120 '
int main(){
    char *s = "abc";
    char *t = "xyz";
    return *t;
}
'

# 複数の文字列リテラル、それぞれ別の文字を返す
assert 97 '
int main(){
    char *s = "abc";
    char *t = "xyz";
    return *s;
}
'

# 文字列の文字と数値の比較
assert 1 '
int main(){
    char *s = "hello";
    if (*s == 104) return 1;
    return 0;
}
'

# 文字列の文字と文字定数の比較
assert 1 '
int main(){
    char *s = "hello";
    if (*s == '\''h'\'') return 1;
    return 0;
}
'

# 宣言と代入を分けた場合
assert 104 '
int main(){
    char *s;
    s = "hello";
    return *s;
}
'

# グローバルchar*に文字列を代入
assert 119 '
char *g;
int main(){
    g = "world";
    return *g;
}
'

# グローバルchar*の文字列をインデックスアクセス
assert 111 '
char *g;
int main(){
    g = "world";
    return g[1];
}
'

# ループで文字列を走査 ('h'=104, 'e'=101, ... 5文字目の'\0'=0)
assert 0 '
int main(){
    char *s = "hello";
    char *p = s;
    while (*p != 0)
        p = p + 1;
    return *p;
}
'

# 文字列長を求める（'\0'まで数える）
assert 5 '
int strlen_impl(char *s){
    int n = 0;
    while (*(s + n) != 0)
        n = n + 1;
    return n;
}
int main(){
    return strlen_impl("hello");
}
'

assert 5 '
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
'
