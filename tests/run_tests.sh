#!/bin/bash
# tests/test*.c を番号順に走らせるテストランナー。
#
# 各テストファイルは先頭に `// EXPECT: N` コメントを持っており、
# その N を mcc でコンパイル・リンク・実行した exit code と比較する。
#
# 現状の mcc はまだ argv 直接入力方式なので、Phase 1 (ファイル入力対応)
# が完了するまでは各テストを cat したものをそのまま mcc に渡している。
# Phase 1 完了後は MCC_INPUT_MODE=file を指定すると `./mcc <path>` に切り替わる。

set -u

SCRIPT_DIR="$(cd "$(dirname "$0")" && pwd)"
PROJECT_DIR="$(cd "$SCRIPT_DIR/.." && pwd)"

MCC="${MCC:-$PROJECT_DIR/mcc}"
TEST_FUNC_C="${TEST_FUNC_C:-$PROJECT_DIR/test_func.c}"
INPUT_MODE="${MCC_INPUT_MODE:-argv}"  # argv | file

TMP_S="$(mktemp --suffix=.s)"
TMP_BIN="$(mktemp)"
trap 'rm -f "$TMP_S" "$TMP_BIN"' EXIT

if [ ! -x "$MCC" ]; then
    echo "mcc バイナリが見つかりません: $MCC" >&2
    exit 2
fi

# 失敗時にテストファイルのソース本体 (EXPECT/SECTION コメントを除いたもの) と
# 捕捉した stderr を表示するヘルパー。
show_failure() {
    local file="$1"
    local stderr_file="$2"
    echo "  --- source ($file) ---"
    grep -v -E '^// (EXPECT|SECTION):' "$file" | sed 's/^/  | /'
    if [ -s "$stderr_file" ]; then
        echo "  --- stderr ---"
        sed 's/^/  | /' "$stderr_file"
    fi
    echo "  ----------------------"
}

TMP_ERR="$(mktemp)"
trap 'rm -f "$TMP_S" "$TMP_BIN" "$TMP_ERR"' EXIT

total=0
passed=0
failed_files=()

# test*.c を番号順にソート
mapfile -t files < <(ls "$SCRIPT_DIR"/test*.c 2>/dev/null | \
    awk -F'test|\\.c' '{print $(NF-1), $0}' | sort -n | awk '{print $2}')

if [ "${#files[@]}" -eq 0 ]; then
    echo "tests/test*.c が見つかりません" >&2
    exit 2
fi

for f in "${files[@]}"; do
    total=$((total + 1))

    expected="$(grep -m1 '^// EXPECT:' "$f" | sed -E 's|^// EXPECT:[[:space:]]*||')"
    section="$(grep -m1 '^// SECTION:' "$f" | sed -E 's|^// SECTION:[[:space:]]*||')"
    name="$(basename "$f")"

    if [ -z "$expected" ]; then
        echo "[SKIP] $name: EXPECT コメントなし"
        continue
    fi

    : > "$TMP_ERR"

    # mcc にソースを渡してアセンブリを生成
    if [ "$INPUT_MODE" = "file" ]; then
        if ! "$MCC" "$f" > "$TMP_S" 2>"$TMP_ERR"; then
            echo "[FAIL] $name ($section): mcc がエラー終了"
            show_failure "$f" "$TMP_ERR"
            failed_files+=("$name")
            continue
        fi
    else
        # 現状の argv モード: EXPECT/SECTION コメントを削除して 1 引数に連結
        src="$(grep -v -E '^// (EXPECT|SECTION):' "$f")"
        if ! "$MCC" "$src" > "$TMP_S" 2>"$TMP_ERR"; then
            echo "[FAIL] $name ($section): mcc がエラー終了"
            show_failure "$f" "$TMP_ERR"
            failed_files+=("$name")
            continue
        fi
    fi

    # アセンブル + リンク
    if ! cc -o "$TMP_BIN" "$TMP_S" "$TEST_FUNC_C" 2>"$TMP_ERR"; then
        echo "[FAIL] $name ($section): cc がエラー終了"
        show_failure "$f" "$TMP_ERR"
        failed_files+=("$name")
        continue
    fi

    # 実行 → exit code を取得
    "$TMP_BIN"
    actual="$?"

    if [ "$actual" = "$expected" ]; then
        passed=$((passed + 1))
        echo "[PASS] $name ($section) => $actual"
    else
        echo "[FAIL] $name ($section): expected=$expected actual=$actual"
        show_failure "$f" "$TMP_ERR"
        failed_files+=("$name")
    fi
done

echo
echo "==================================================="
echo "結果: $passed / $total 成功"
if [ "${#failed_files[@]}" -gt 0 ]; then
    echo "失敗したテスト:"
    for f in "${failed_files[@]}"; do
        echo "  - $f"
    done
    exit 1
fi
echo "==================================================="
