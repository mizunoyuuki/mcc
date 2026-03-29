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

### 目標

セルフコンパイル（mizucc 自身を mizucc でコンパイルできるようにすること）を最終目標としています。

### アルゴリズム・データ構造の学習材料として

コンパイラの実装を通じて、以下のアルゴリズム・データ構造を実践的に学ぶことができます。

| 分野 | アルゴリズム / データ構造 | コンパイラでの用途 |
|------|--------------------------|-------------------|
| 構文解析 | 再帰下降構文解析 (Recursive Descent Parsing) | パーサの中核。演算子の優先順位を関数の呼び出し階層で表現 |
| データ構造 | 連結リスト (Linked List) | トークン列、ローカル変数の管理 |
| データ構造 | 抽象構文木 / AST (Tree) | パース結果の中間表現。ツリー走査でコード生成 |
| データ構造 | スタック (Stack) | スタックマシンモデルによるコード生成、x86 のコールスタック |
| データ構造 | ハッシュテーブル (Hash Table) | シンボルテーブル（変数・関数名の高速検索） |
| グラフ理論 | 有向非巡回グラフ / DAG | 共通部分式の除去、式の最適化 |
| グラフ理論 | グラフ彩色 (Graph Coloring) | レジスタ割り当て最適化 |
| 探索 | 深さ優先探索 / DFS (Depth-First Search) | AST の走査、制御フローの解析 |
| 最適化 | データフロー解析 (Data-Flow Analysis) | 定数畳み込み、到達定義解析、不要コード除去 |
| 最適化 | ピープホール最適化 (Peephole Optimization) | 生成された命令列の局所的な改善 |
| 文字列処理 | 有限オートマトン / DFA | トークナイザ（字句解析）の理論的基盤 |
| メモリ管理 | アリーナアロケータ (Arena Allocator) | コンパイラ内部のメモリ管理効率化 |

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

### Goal

The ultimate goal is **self-hosting** — compiling mizucc with mizucc itself.

### As a Learning Resource for Algorithms & Data Structures

Building a compiler provides hands-on experience with the following algorithms and data structures:

| Area | Algorithm / Data Structure | Use in Compiler |
|------|---------------------------|-----------------|
| Parsing | Recursive Descent Parsing | Core of the parser. Operator precedence via call hierarchy |
| Data Structure | Linked List | Token sequences, local variable management |
| Data Structure | Abstract Syntax Tree (Tree) | Intermediate representation. Tree traversal for code generation |
| Data Structure | Stack | Stack-machine code generation model, x86 call stack |
| Data Structure | Hash Table | Symbol table (fast lookup of variables and functions) |
| Graph Theory | Directed Acyclic Graph (DAG) | Common subexpression elimination, expression optimization |
| Graph Theory | Graph Coloring | Register allocation optimization |
| Search | Depth-First Search (DFS) | AST traversal, control-flow analysis |
| Optimization | Data-Flow Analysis | Constant folding, reaching definitions, dead code elimination |
| Optimization | Peephole Optimization | Local improvement of generated instruction sequences |
| String Processing | Finite Automaton (DFA) | Theoretical foundation of the tokenizer (lexer) |
| Memory Management | Arena Allocator | Efficient internal memory management |

### Usage

```bash
make          # Build the compiler (generates ./mcc)
make test     # Build and run all tests
make clean    # Remove build artifacts

./mcc "int main(){ return 1+2; }"   # Outputs x86-64 assembly to stdout
```
