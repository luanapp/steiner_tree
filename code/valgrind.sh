#!/bin/sh
# vim: set autoindent smartindent tabstop=2 shiftwidth=2 expandtab filetype=sh:

function vgrun
{
  local COMMAND="$1"
  local NAME="$2"
  [[ -n "$COMMAND" ]] || { echo "Syntax: vgrun <command> <name>"; return; }
  [[ -n "$NAME" ]] || { echo "Syntax vgrun <command> <name>"; return; }
  valgrind \
        --leak-check=full --error-limit=no --track-origins=yes \
        --undef-value-errors=yes --log-file=valgrind-${NAME}.log \
        --read-var-info=yes \
        $COMMAND | tee valgrind-${NAME}-output.log 2>&1
}

function vgtrace
{
  local COMMAND="$1"
  local NAME="$2"
  [[ -n "$COMMAND" ]] || { echo "Syntax: vgtrace <command> <name>"; return; }
  [[ -n "$NAME" ]] || { echo "Syntax vgtrace <command> <name>"; return; }
  valgrind \
        --leak-check=full --error-limit=no --track-origins=yes \
        --undef-value-errors=yes --log-file=valgrind-${NAME}.log \
        --read-var-info=yes --trace-children=yes \
        "$COMMAND" | tee valgrind-${NAME}-output.log 2>&1
}

function vgdbg
{
  [[ -n "$*" ]] || { echo "Syntax: vgrun <command>"; return; }
  valgrind \
        --leak-check=full --error-limit=no --track-origins=yes \
        --undef-value-errors=yes --read-var-info=yes --db-attach=yes \
        "$@"
}

[[ $1 == "vgrun" ]] && vgrun $2 $3
[[ $1 == "vgtrace" ]] && vgtrace $2 $3
[[ $1 == "vgdbg" ]] && vgdbg $2 $3
