# otest2

_OTest2_ is a C++ framework for writing of automated tests (unit tests, integration tests, etc.).

Design goals of the framework are:

1. getting rid of writing boiler plate code,
2. extensibility of the framework for different development environments,
3. basic usability in IDEs.

The first goal is reached by a processor based on the _libclang_ which offers full C++ parser. Hence, there are
no limits how the tests are written. Furthermore, the full parser allows many advanced features like fixture importing
and assserted try/catch blocks.

In the sake of the second goal, the framework allows to make own implementation of several crucial parts: user can
define own assertions, own catcher of unhandled exceptions, own main loop or own test reporter. So the framework can
be easily integrated into your specific development environment.

The last goal means working IDE functions like code assistance with the test DSL files. The test files are valid
C++ code so everything works instantly.

## How to build

The framework strongly depends on the _libclang_ library. Unfortunatelly, there are no good packages of it and
the library must be build manually.

### Building Clang

1. Download llvm and clang in version 7.X.X from the [LLVM project site](http://releases.llvm.org/) (different
   version will work very unlikely).
2. Unpack both archives. The unpacked clang must be placed just **next** the unpacked LLVM and must be renamed to _clang_.
3. Create another directory (for example _build_) and change current working dir to.
4. Run cmake: 
   ```
   cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_USE_LINKER=gold path_to_llvm
   ```
   Note: don't try to build the _Debug_ type unless you have much much memory.
5. Then build the libraries:
   ```
   make
   ````
6. At the end install the clang somewhere
   ```
   cmake -DCMAKE_INSTALL_PREFIX=somewhere_path -P cmake_install.cmake
   ```
### Building OTest2

Building of the framework is now easy:
```
scons --with-llvm=path-to-llvm-config
```
The _path-to-llvm-config_ is a path to the _llvm-config_ binary. It is installed in the _bin_ subdirectory of the installed
clang project. If the utility can be found by searching in the _PATH_ variable the switch may be omitted.

If you want to run the selftest suite use this command
```
scons --with-llvm=path-to-llvm-config check
```
The processor needs to know paths to system headers. If compilation of the test suite fails try to fill correct paths
in the root _SConstruct_ file.
