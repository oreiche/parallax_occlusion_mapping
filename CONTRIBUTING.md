# CONTRIBUTING
## Coding style
Adhere to [Google's C++ Style Guide](https://google.github.io/styleguide/cppguide.html).

## Static code analysis
Download and install [LLVM 11.0.0](https://releases.llvm.org/download.html) for your platform. Make sure `clang-format` and `clang-tidy` are available in your `PATH`.

**Alternatively, on Linux 64-bit**, you can skip the download and let CMake set up the toolchain for you:
~~~sh
cmake <PATH_TO_SOURCE> \
      -DTOOLCHAIN_FILE=cmake/toolchain/clang11_linux_x86_64.cmake
export PATH=$(pwd)/externals/llvm_toolchain/bin:$PATH
~~~

### Detecting issues
To detect issues, run the following commands in the project's root directory.

#### On Unix
~~~sh
bash .github/run-clang-format.sh src/ -e test.*data -v
bash .github/run-clang-tidy.sh src/ -e test.*data -v
~~~

#### On Windows
**Requires `bash.exe` in `PATH` (e.g., from WSL or Git Bash)**
~~~powershell
powershell.exe .github\run-clang-format.ps1 src/ -e test.*data -v
powershell.exe .github\run-clang-tidy.ps1 src/ -e test.*data -v
~~~

#### With CMake/CTest
Enable static analysis tests and run them.
~~~sh
cmake <PATH_TO_SOURCE> -DRUN_ANALYSIS=ON
ctest -C <CONFIG> -R static_analysis_clang-format
ctest -C <CONFIG> -R static_analysis_clang-tidy
~~~

### Fixing issues
In many cases, issues can be fixed automatically by the formatter/linter. To automatically fix issues, run the following commands in the project's root directory.

#### On Unix
~~~sh
bash .github/run-clang-format.sh src/ -e test.*data -f
bash .github/run-clang-tidy.sh src/ -e test.*data -f
~~~

#### On Windows
**Requires `bash.exe` in `PATH` (e.g., from WSL or Git Bash)**
~~~powershell
powershell.exe .github\run-clang-format.ps1 src/ -e test.*data -f
powershell.exe .github\run-clang-tidy.ps1 src/ -e test.*data -f
~~~
