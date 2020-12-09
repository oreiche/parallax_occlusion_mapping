$ErrorActionPreference = "Stop"

bash.exe -c "export WIN32_EXT=.exe; .github/run-clang-tidy.sh $args"
