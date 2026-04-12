# mizucc (mcc)

## 日本語

mizucc は、C言語のサブセットを x86-64 アセンブリ（Intel構文）にコンパイルする自作Cコンパイラです。

[低レイヤを知りたい人のためのCコンパイラ作成入門](https://www.sigbus.info/compilerbook) を参考に開発しています。

### 現在サポートしている機能

- 整数リテラル、文字定数（`'a'`, `'\n'`, `'\t'`, `'\r'`, `'\0'`, `'\\'`, `'\''`, `'\"'`）
- 文字列リテラル（`"hello"` — `.rodata` セクションに配置）
- 四則演算（`+`, `-`, `*`, `/`）
- 比較演算子（`==`, `!=`, `<`, `<=`, `>`, `>=`）
- 型: `int`, `char`, ポインタ（多重可）、固定長配列、構造体（`struct`）
- 構造体のタグ定義（`struct A { int a; char b; };`）、メンバのアラインメント・パディング対応
- 構造体のローカル変数宣言（`struct A x;`）、構造体ポインタ変数宣言（`struct A *p;`）
- ローカル変数の宣言と初期化
- グローバル変数（`int`, `char`, ポインタ, 配列）
- 代入
- `return` 文
- 制御構文（`if` / `else`, `while`, `for`）
- ブロック文 `{}`
- 関数の定義と呼び出し（最大6引数、ポインタ引数・ポインタ戻り値に対応）
- 単項演算子 `&`（アドレス取得）、`*`（デリファレンス）
- ポインタ算術（`ptr + int` で型サイズに応じた自動スケーリング）
- 配列の添字アクセス `a[i]`（`*(a+i)` への糖衣）
- 配列からポインタへの暗黙変換（array decay）
- `sizeof` 演算子（式・型の両方、構造体変数にも対応、コンパイル時に定数に畳み込む）
- 論理否定演算子 `!`
- コメント（`//` 一行コメント、`/* */` ブロックコメント）
- スコープ管理（ブロックスコープ、変数のシャドーイング）

### コンパイラの構成

以下のパイプラインで構成されています。

1. **字句解析** (`tokenizer.c`) — 入力文字列をトークン連結リストに変換
2. **構文解析** (`parser.c`) — 再帰下降パーサで AST を構築
3. **意味解析** (`type.c`) — 全ノードに型を付与し、ポインタ算術のスケーリングや array decay を行う
4. **最適化** (`optimize.c`) — AST レベルの最適化（現在はパススルー）
5. **コード生成** (`generator.c`) — スタックマシンモデルで x86-64 アセンブリ（Intel構文）を出力

### 目標

セルフコンパイル（mizucc 自身を mizucc でコンパイルできるようにすること）を最終目標としています。

### アルゴリズム・データ構造の学習材料として

コンパイラの実装を通じて、以下のアルゴリズム・データ構造を実践的に学ぶことができます。

| 分野 | アルゴリズム / データ構造 | コンパイラでの用途 | 実装状況 |
|------|--------------------------|-------------------|:--------:|
| 構文解析 | 再帰下降構文解析 (Recursive Descent Parsing) | パーサの中核。演算子の優先順位を関数の呼び出し階層で表現 | ⚪ |
| データ構造 | 連結リスト (Linked List) | トークン列、ローカル/グローバル変数、型情報、文字列リテラルの管理 | ⚪ |
| データ構造 | 抽象構文木 / AST (Tree) | パース結果の中間表現。ツリー走査でコード生成 | ⚪ |
| データ構造 | スタック (Stack) | スタックマシンモデルによるコード生成、x86 のコールスタック | ⚪ |
| データ構造 | ハッシュテーブル (Hash Table) | シンボルテーブル（変数・関数名の高速検索） | × |
| 型システム | 型推論・型変換 (Type System) | ポインタ算術のスケーリング、array-to-pointer decay | ⚪ |
| グラフ理論 | 有向非巡回グラフ / DAG | 共通部分式の除去、式の最適化 | × |
| グラフ理論 | グラフ彩色 (Graph Coloring) | レジスタ割り当て最適化 | × |
| 探索 | 深さ優先探索 / DFS (Depth-First Search) | AST の走査、制御フローの解析 | ⚪ |
| 最適化 | データフロー解析 (Data-Flow Analysis) | 定数畳み込み、到達定義解析、不要コード除去 | × |
| 最適化 | ピープホール最適化 (Peephole Optimization) | 生成された命令列の局所的な改善 | × |
| 文字列処理 | 有限オートマトン / DFA | トークナイザ（字句解析）の理論的基盤 | △ |
| メモリ管理 | アリーナアロケータ (Arena Allocator) | コンパイラ内部のメモリ管理効率化 | × |

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

- Integer literals, character constants (`'a'`, `'\n'`, `'\t'`, `'\r'`, `'\0'`, `'\\'`, `'\''`, `'\"'`)
- String literals (`"hello"` — placed in the `.rodata` section)
- Arithmetic operators (`+`, `-`, `*`, `/`)
- Comparison operators (`==`, `!=`, `<`, `<=`, `>`, `>=`)
- Types: `int`, `char`, pointers (multi-level), fixed-size arrays, structs (`struct`)
- Struct tag definitions (`struct A { int a; char b; };`) with member alignment and padding
- Struct local variable declarations (`struct A x;`), struct pointer variable declarations (`struct A *p;`)
- Local variable declaration and initialization
- Global variables (`int`, `char`, pointers, arrays)
- Assignment
- `return` statement
- Control flow (`if` / `else`, `while`, `for`)
- Block statements `{}`
- Function definitions and calls (up to 6 arguments, supports pointer arguments and pointer return types)
- Unary `&` (address-of) and `*` (dereference)
- Pointer arithmetic with automatic scaling based on the pointed-to type
- Array subscript `a[i]` (desugared to `*(a+i)`)
- Implicit array-to-pointer decay
- `sizeof` operator (both expressions and types, including struct variables, folded at compile time)
- Logical NOT operator `!`
- Comments (`//` single-line, `/* */` block comments)
- Scope management (block scope, variable shadowing)

### Compiler Architecture

The compiler is organized as a multi-stage pipeline:

1. **Lexer** (`tokenizer.c`) — converts the input string into a linked list of tokens
2. **Parser** (`parser.c`) — recursive-descent parser that builds the AST
3. **Semantic analysis** (`type.c`) — annotates every node with a type, handles pointer arithmetic scaling and array decay
4. **Optimizer** (`optimize.c`) — AST-level optimization (currently pass-through)
5. **Code generator** (`generator.c`) — emits x86-64 assembly (Intel syntax) using a stack-machine model

### Goal

The ultimate goal is **self-hosting** — compiling mizucc with mizucc itself.

### As a Learning Resource for Algorithms & Data Structures

Building a compiler provides hands-on experience with the following algorithms and data structures:

| Area | Algorithm / Data Structure | Use in Compiler | Status |
|------|---------------------------|-----------------|:------:|
| Parsing | Recursive Descent Parsing | Core of the parser. Operator precedence via call hierarchy | ⚪ |
| Data Structure | Linked List | Tokens, local/global variables, type info, string literals | ⚪ |
| Data Structure | Abstract Syntax Tree (Tree) | Intermediate representation. Tree traversal for code generation | ⚪ |
| Data Structure | Stack | Stack-machine code generation model, x86 call stack | ⚪ |
| Data Structure | Hash Table | Symbol table (fast lookup of variables and functions) | × |
| Type System | Type inference & conversion | Pointer arithmetic scaling, array-to-pointer decay | ⚪ |
| Graph Theory | Directed Acyclic Graph (DAG) | Common subexpression elimination, expression optimization | × |
| Graph Theory | Graph Coloring | Register allocation optimization | × |
| Search | Depth-First Search (DFS) | AST traversal, control-flow analysis | ⚪ |
| Optimization | Data-Flow Analysis | Constant folding, reaching definitions, dead code elimination | × |
| Optimization | Peephole Optimization | Local improvement of generated instruction sequences | × |
| String Processing | Finite Automaton (DFA) | Theoretical foundation of the tokenizer (lexer) | △ |
| Memory Management | Arena Allocator | Efficient internal memory management | × |

### Usage

```bash
make          # Build the compiler (generates ./mcc)
make test     # Build and run all tests
make clean    # Remove build artifacts

./mcc "int main(){ return 1+2; }"   # Outputs x86-64 assembly to stdout
```
