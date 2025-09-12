#!/usr/bin/env bash
set -euo pipefail

# Minishell heredoc + redirection stress case reproduced from your session
# Runs under valgrind and feeds predefined heredoc bodies.

ROOT_DIR="$(cd -- "$(dirname -- "${BASH_SOURCE[0]}")"/.. && pwd)"
cd "$ROOT_DIR"

# Ensure required input files exist for redirections used in the command
: > 1      # input file used by <1
: > 9a     # input file used by <9a
rm -f bua buaa a 2>/dev/null || true

VAL_CMD=(valgrind --leak-check=full --show-leak-kinds=all \
  --suppressions=readline.supp --track-fds=yes ./minishell)

cat > ._heredoc_redirs_input <<'EOF'
<<a <<a <1 >bua >buaa <bua >>bua <<1 <9a >>a cat
sad
sa
ds
d
a
sdsd
dsf
sdf
sd
f
a
dsfsdfsdf
1
exit
EOF

"${VAL_CMD[@]}" < ._heredoc_redirs_input

echo
echo "--- Files after run ---"
ls -l -- bua buaa a 1 9a 2>/dev/null || true

