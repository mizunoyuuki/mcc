# mizucc (mcc)

## 日本語

mizucc は、C言語のサブセットを x86-64 アセンブリ（Intel構文）にコンパイルする自作Cコンパイラです。

[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook) を参考に開発しています。

### 現在サポートしている機能

- 整数リテラル
- 四則演算（`+`, `-`, `*`, `/`）
- 比較演算子（`==`, `!=`, `<`, `<=`, `>`, `>=`）
- ローカル変数（`int`, `char`, ポインタ）
- 代入
- `return` 文
- 制御構文（`if` / `else`, `while`, `for`）
- ブロック文 `{}`
- 関数の定義と呼び出し
- ポインタ（`&`, `*`）
- `sizeof` 演算子

### 使い方

```bash
make          # コンパイラをビルド（./mcc が生成される）
make test     # ビルドして全テスト実行
make clean    # ビルド成果物を削除

./mcc "int main(){ return 1+2; }"   # x86-64 アセンブリが標準出力に出力される
```

---

## English

mizucc is a small C compiler that compiles a subset of C into x86-64 assembly (Intel syntax).

This project is based on [compilerbook](https://www.sigbus.info/compilerbook) by Rui Ueyama.

### Currently Supported Features

- Integer literals
- Arithmetic operators (`+`, `-`, `*`, `/`)
- Comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- Local variables (`int`, `char`, pointers)
- Assignment
- `return` statement
- Control flow (`if` / `else`, `while`, `for`)
- Block statements `{}`
- Function definitions and calls
- Pointers (`&`, `*`)
- `sizeof` operator

### Usage

```bash
make          # Build the compiler (generates ./mcc)
make test     # Build and run all tests
make clean    # Remove build artifacts

./mcc "int main(){ return 1+2; }"   # Outputs x86-64 assembly to stdout
```
