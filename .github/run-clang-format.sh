#!/bin/bash

set -euo pipefail

export PATH=".externals/llvm_toolchain/bin:$PATH"

readonly STORAGE_DIR="${TMPDIR:-/tmp}/run-clang-format_$USER"
readonly EXEC_DIR="$STORAGE_DIR/exec.$$"

function usage() {
  echo "Usage: $(basename $0) [SRC_DIR] [OPTION]..."
  echo "Run clang-format on every C/C++ file in SRC_DIR."
  echo
  echo "Possible OPTIONs are:"
  echo "  -e REGEX  exclude regex pattern"
  echo "  -f        fix formats of all files"
  echo "  -v        verbose, print format diff"
}

function run_clang_format() {
  local SRC_DIR="$(pwd)"
  local EXCL_REGX=
  local FIX_FORMAT=false
  local VERBOSE=false

  # parse source directory argument
  if [ -n "$1" ] && ! [[ "$1" =~ ^- ]]; then
    SRC_DIR="$1"
    shift
  fi

  # parse remaining arguments
  while getopts "h?e:fv" OPT "$@"; do
    case "${OPT}" in
      h|\?)
        usage
        exit 0
        ;;
      e)
        EXCL_REGX="${OPTARG}"
        ;;
      f)
        FIX_FORMAT=true
        ;;
      v)
        VERBOSE=true
        ;;
    esac
  done

  mkdir -p "$EXEC_DIR"

  if ! command -v clang-format &>/dev/null; then
    echo "Error: Cannot find clang-format in PATH"
    return 1
  fi

  printf "Running clang-format on %s\n" "$SRC_DIR"

  local RETVAL=0 SRC_FILE
  while read -r SRC_FILE; do
    # skip non-C/C++ files
    if ! file "$SRC_FILE" | grep -iq "c source\|c++ source"; then continue; fi

    # skip on exclude regex match
    if [ -n "$EXCL_REGX" ] && [[ "$SRC_FILE" =~ $EXCL_REGX ]]; then continue; fi

    if $FIX_FORMAT; then
      # fix format in-place
      clang-format -verbose -i -style=file "$SRC_FILE"
    else
      # count format errors
      local FMT_ERRS=$(clang-format -style=file \
                                    -output-replacements-xml \
                                    "$SRC_FILE" \
                        | grep -c "replacement offset")
      if [ "$FMT_ERRS" -gt "0" ]; then
        RETVAL=1
        printf "%4d format error(s) in %s\n" "$FMT_ERRS" "$SRC_FILE"
        if ${VERBOSE}; then
          local TMP_FILE="$EXEC_DIR/$(basename "$SRC_FILE")"
          clang-format -style=file "$SRC_FILE" > "$TMP_FILE"
          diff --color -u -p "$SRC_FILE" "$TMP_FILE" || true
        fi
      fi
    fi
  done < <(find "${SRC_DIR}" -type f)

  # cleanup
  rm -rf "$EXEC_DIR"

  return $RETVAL
}

if [[ "$0" == "$BASH_SOURCE" ]]; then
  run_clang_format "$@"
  exit $?
fi
