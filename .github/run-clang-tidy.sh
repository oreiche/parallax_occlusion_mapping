#!/bin/bash

set -euo pipefail

readonly CC_FLAGS=(
  "-std=c++17"
  "-DGLEW_NO_GLU"
  "-DWITH_OPENGL"
  "-I."
  "-I.externals/github_gsl_lite/include"
  "-I.externals/github_catch2/single_include"
  "-I.externals/github_glew/include"
  "-I.externals/github_glfw/include"
  "-I.externals/github_glm"
  "-I.externals/github_sail/include"
  "-I.externals/github_freeimage/Source"
  "-I.externals/github_freeimage/Wrapper/FreeImagePlus"
)

readonly CLANG=clang${WIN32_EXT:-}
readonly CLANG_TIDY=clang-tidy${WIN32_EXT:-}

readonly STORAGE_DIR="${TMPDIR:-/tmp}/run-clang-tidy_$USER"
readonly CACHE_DIR="$STORAGE_DIR/cache"
readonly EXEC_DIR="$STORAGE_DIR/exec.$$"
readonly LOG_DIR="$EXEC_DIR/logs"
readonly FAIL_FLAG="$EXEC_DIR/failed"

function usage() {
  echo "Usage: $(basename $0) [SRC_DIR] [OPTION]..."
  echo "Run $CLANG_TIDY on every C/C++ file in SRC_DIR."
  echo
  echo "Possible OPTIONs are:"
  echo "  -e REGEX  exclude regex pattern"
  echo "  -f        fix issues of all files"
  echo "  -j N      jobs to run in parallel"
  echo "  -n        no cache from previous runs"
  echo "  -s        stop and exit on first error"
  echo "  -v        verbose, print errors and warnings"
}

function compute_hash() {
  echo "$1" | sha1sum | cut -d' ' -f1
}

function job_limit_reached() {
  local JOB_LIMIT=$1
  shift
  local INPUT_PIDS=("$@")
  local COUNT=0
  for PID in "${INPUT_PIDS[@]}"; do
    if ps -p $PID &>/dev/null; then COUNT=$(($COUNT+1)); fi
    if [ $COUNT -ge $JOB_LIMIT ]; then return 0; fi
  done
  return 1
}

function collect_running_pids() {
  local INPUT_PIDS=("$@")
  local RUNNING_PIDS=()
  for PID in "${INPUT_PIDS[@]}"; do
    if ps -p $PID &>/dev/null; then
      RUNNING_PIDS+=("$PID")
    fi
  done
  echo "${RUNNING_PIDS[@]}"
}

function parse_issues_from_log() {
  local LOG_FILE=$1
  local FILE_FILTER=$2
  local PATTERN="[0-9]\+:[0-9]\+"

  # count all errors, also from file dependencies
  local NUM_ERRS=$(grep -c "$PATTERN:\ error:\ " "$LOG_FILE")

  # count only those warnings and notes that are directly related to the file
  local NUM_WARNS=$(grep -c "$FILE_FILTER:$PATTERN:\ warning:\ " "$LOG_FILE")
  local NUM_NOTES=$(grep -c "$FILE_FILTER:$PATTERN:\ note:\ " "$LOG_FILE")

  local NUM_TOTAL=$(($NUM_ERRS+$NUM_WARNS+$NUM_NOTES))
  echo $NUM_TOTAL
  if [ $NUM_TOTAL -gt 0 ]; then return 0; fi
  return 1
}

function exec_clang_tidy() {
  local SRC_FILE="$1"
  local VER_HASH="$2"
  local CFG_HASH="$3"
  local FIX_ISSUES=${4:-false}
  local NO_CACHE=${5:-false}
  local JOB_PID=$BASHPID

  if $FIX_ISSUES; then
    # fix issues in-place
    $CLANG_TIDY -fix-errors ${CC_FLAGS[@]/#/-extra-arg=} "$SRC_FILE" \
              &>/dev/null || true
  else
    local CACHE_ENTRY=

    if ! $NO_CACHE; then
      # compute hash from file content with all headers included
      local FILE_CONTENT="$($CLANG -E ${CC_FLAGS[@]} "${SRC_FILE}" 2>/dev/null)"
      local FILE_HASH="$(compute_hash "$FILE_CONTENT")"

      # compute cache entry and exit on cache hit
      CACHE_ENTRY="$CACHE_DIR/$(compute_hash "$VER_HASH+$CFG_HASH+$FILE_HASH")"
      if [ -f "$CACHE_ENTRY" ]; then return; fi
    fi

    # exec clang-tidy and store log for this job
    $CLANG_TIDY ${CC_FLAGS[@]/#/-extra-arg=} "$SRC_FILE" \
              2>/dev/null >"$LOG_DIR/$JOB_PID" || true

    # if issues are related to this file (and not only to its dependencies)
    if parse_issues_from_log "$LOG_DIR/$JOB_PID" "$SRC_FILE" &>/dev/null; then
      # record failure and store this job's pid (atomic create and rename dance)
      echo $JOB_PID > "$FAIL_FLAG.$JOB_PID"
      mv --no-clobber "$FAIL_FLAG.$JOB_PID" "$FAIL_FLAG"
    else
      # record success by creating cache entry and remove log
      if [ -n "$CACHE_ENTRY" ]; then touch "$CACHE_ENTRY"; fi
      rm -f "$LOG_DIR/$JOB_PID"
    fi
  fi
}

function run_clang_tidy() {
  local SRC_DIR="${1:-}"
  local EXCL_REGX=
  local FIX_ISSUES=false
  local NO_CACHE=false
  local JOB_LIMIT=$(nproc --all)
  local STOP_ON_FAILURE=false
  local VERBOSE=false

  # parse source directory argument
  if [ -z "$SRC_DIR" ]; then
    SRC_DIR=$(pwd)
  elif ! [[ "$1" =~ ^- ]]; then
    shift
  fi

  # parse remaining arguments
  while getopts "h?e:fj:nsv" OPT "$@"; do
    case "${OPT}" in
      h|\?)
        usage
        exit 0
        ;;
      e)
        EXCL_REGX="${OPTARG}"
        ;;
      f)
        FIX_ISSUES=true
        ;;
      j)
        JOB_LIMIT="${OPTARG}"
        ;;
      n)
        NO_CACHE=true
        ;;
      s)
        STOP_ON_FAILURE=true
        ;;
      v)
        VERBOSE=true
        ;;
    esac
  done

  mkdir -p "$LOG_DIR"
  mkdir -p "$CACHE_DIR"

  if (! command -v $CLANG || ! command -v $CLANG_TIDY) &>/dev/null; then
    echo "Error: Cannot find $CLANG or $CLANG_TIDY in PATH"
    return 1
  fi

  printf "Running $CLANG_TIDY on %s\n" "$SRC_DIR"

  local RETVAL=0 CHILD_PIDS=() PID_FILE_MAP=() SRC_FILE
  local VER_HASH="$(compute_hash "$($CLANG_TIDY -version)")"
  local CFG_HASH="$(compute_hash "$($CLANG_TIDY -dump-config)")"

  while read -r SRC_FILE; do
    # skip non-C/C++ files
    if ! file "$SRC_FILE" | grep -iq "c source\|c++ source"; then continue; fi

    # skip on exclude regex match
    if [ -n "$EXCL_REGX" ] && [[ "$SRC_FILE" =~ $EXCL_REGX ]]; then continue; fi

    # adhere to job limit
    while job_limit_reached $JOB_LIMIT "${CHILD_PIDS[@]}"; do
      # wait for at least one job to finish
      wait -fn ${CHILD_PIDS[@]}
    done

    # early exit on failure
    if $STOP_ON_FAILURE && [ -f "$FAIL_FLAG" ]; then break; fi

    # execute clang-tidy as background job
    exec_clang_tidy "$SRC_FILE" $VER_HASH $CFG_HASH $FIX_ISSUES $NO_CACHE &

    # record new child pid
    CHILD_PIDS=($(collect_running_pids "${CHILD_PIDS[@]}" "$!"))
    PID_FILE_MAP[$!]="$SRC_FILE"
  done < <(find "${SRC_DIR}" -type f)

  # wait for all remaining children to finish
  wait -f ${CHILD_PIDS[@]}

  if [ -f "$FAIL_FLAG" ]; then
    RETVAL=1

    if $STOP_ON_FAILURE; then
      # remove all but first failed pid's log
      local FAIL_PID=$(cat "$FAIL_FLAG")
      echo "$(find "$LOG_DIR" -type f | grep -v $FAIL_PID)" | xargs rm -f
    fi

    for PID in $(ls "$LOG_DIR"); do
      # report issues from logs
      local SRC_FILE="${PID_FILE_MAP[$PID]}"
      local NUM_ISSUES=$(parse_issues_from_log "$LOG_DIR/$PID" "$SRC_FILE")
      printf "%4d issue(s) in %s\n" $NUM_ISSUES "$SRC_FILE"
      if $VERBOSE; then cat "$LOG_DIR/$PID"; fi
    done
  fi

  # cleanup
  rm -rf "$EXEC_DIR"

  return $RETVAL
}

if [[ "$0" == "$BASH_SOURCE" ]]; then
  run_clang_tidy "$@"
  exit $?
fi
