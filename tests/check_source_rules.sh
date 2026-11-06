#!/usr/bin/env bash
# Lightweight student feedback, not a proof of allowed data-structure use.
set -euo pipefail
if [[ $# -eq 0 ]]; then
  echo 'Usage: tests/check_source_rules.sh SOURCE...' >&2
  exit 2
fi
for file in "$@"; do
  if [[ ! -f "$file" ]]; then
    echo "[RULE CHECK ERROR] Missing source file: $file" >&2
    exit 2
  fi
  # Mirrors Assignment 1: strip // comments, then check includes and common calls.
  # Block comments / aliases / generated code are subject to additional manual review.
  source_without_line_comments=$(sed -E 's,//.*$,,' "$file")
  forbidden_headers='^[[:space:]]*#[[:space:]]*include[[:space:]]*[<"](map|set|unordered_map|unordered_set|ext/pb_ds/|boost/)' 
  forbidden_containers='(^|[^[:alnum:]_])(std[[:space:]]*::[[:space:]]*)?(map|multimap|unordered_map|unordered_multimap|set|multiset|unordered_set|unordered_multiset|priority_queue)[[:space:]]*<' 
  forbidden_heap_functions='(^|[^[:alnum:]_])(std[[:space:]]*::[[:space:]]*)?(make_heap|push_heap|pop_heap|sort_heap|is_heap|is_heap_until)[[:space:]]*\('
  if printf '%s\n' "$source_without_line_comments" | grep -nE "$forbidden_headers" >/dev/null ||
     printf '%s\n' "$source_without_line_comments" | grep -nE "$forbidden_containers" >/dev/null ||
     printf '%s\n' "$source_without_line_comments" | grep -nE "$forbidden_heap_functions" >/dev/null; then
     echo "[RULE CHECK FAILED] $file uses a disallowed data-structure substitute or STL heap algorithm." >&2
     echo 'Allowed examples: std::vector, std::string. Implement the required BST/AVL/Heap algorithms yourself.' >&2
     exit 1
  fi
done
