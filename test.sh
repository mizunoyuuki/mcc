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
