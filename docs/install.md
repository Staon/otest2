---
title: Install Guide
permalink: /install-guide/
---

## From Sources

The framework strongly depends on the libclang library. As far as I know
there is no usable binary distribution of the library and it must be built
manually.

There are some needed dependencies:
* [cmake](https://cmake.org/) (>= 3.17)
* libtinfo or libncurses
* [libbz2](https://www.sourceware.org/bzip2/)
* [Boost Endian Library](https://www.boost.org/doc/libs/1_63_0/libs/endian/doc/index.html)
* [Pugi XML](https://pugixml.org/)

If you work on Debian Stretch all of them but the cmake are in the system
repository with an appropriate version.

If you want to compile the examples one of them depends on:
* [pkg-config](https://www.freedesktop.org/wiki/Software/pkg-config/)
* [libevent](https://libevent.org/)

### Building Clang

* Download llvm and clang in version 7.X.X from the 
  [LLVM project site](http://releases.llvm.org/) (different version will work
  very unlikely).
* Unpack both archives. The unpacked clang must be placed just __next__
  the unpacked LLVM and __must be renamed__ to _clang_.
* Create another directory (for example _build_) and change current working
   dir to.
* Run cmake:
  ```console
  $ cmake -DCMAKE_BUILD_TYPE=Release -DLLVM_ENABLE_PROJECTS=clang -DLLVM_USE_LINKER=gold <path_to_llvm>
  ```
  Note: don't attempt to compile the _Debug_ build type unless you've got really
  much RAM.
* Then build the libraries:
  ```console
  $ make
  ```
* At the end install the clang somewhere
  ```console
  $ cmake -DCMAKE_INSTALL_PREFIX=<somewhere_path> -P cmake_install.cmake
  ```
  Be sure the installation location is inside the searching path list of the
  cmake's [find_package](https://cmake.org/cmake/help/latest/command/find_package.html)
  directive. Otherwise, cmake won't be able to find it and compilation of
  OTest2 will fail.

### Building OTest2

* Download and unpack the OTest2 sources:
  * clone the [OTest2 repository]({{ site.repositoryurl }})
  * download a source package from [GitHub]({{ site.repositoryurl }}/releases/) 
* Create another directory (for example _build_) and change current working
  directory to.
* Run cmake:
  ```console
  $ cmake <path_to_otest2>
  ```
* Then build the framework:
  ```console
  $ make
  ```
* Optionally, but strongly recommended, run the self-test suite
  ```console
  $ make check
  ```
* Install the framework:
  ```console
  $ cmake -DCMAKE_INSTALL_PREFIX=<somewhere_path> -P cmake_install.cmake
  ```
  If you intent to use the otest2 with the cmake building system (strongly
  recommended) be sure the installation path is inside the searching list
  of the cmake's [find_package](https://cmake.org/cmake/help/latest/command/find_package.html)
  directive.

## Binary Packages

There are prepared binary packages for 64-bits Linux. Download any binary
package from [GitHub]({{ site.repositoryurl }}/releases/).

## Configuration

The OTest2 pre-processor needs to know locations of header files installed
in the system. Most of them is able to find itself but there are some locations
which must be configured[^1].

The pre-processor searches for configuration files at

1. _/etc/otest2_includes.conf_
2. _${HOME}/.otest2\_includes_
3. _${PWD}/.otest2\_includes_

The configuration file is a simple text file: one include path per line.

On a gcc-based system you can create the configuration file by a simple
script _otest2_discovery.sh_ which prints default include paths:
```console
$ otest2_discover.sh > ~/.otest2_includes
```

The generated testing binaries have some dependencies on libraries which
must be installed in the system:

* libtinfo
* [libbz2](https://www.sourceware.org/bzip2/)
* [Pugi XML](https://pugixml.org/)

[^1]: in my case it cannot find the _stdarg.h_ header because it's located
      in a gcc specific path.