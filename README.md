#  Musl Libc LLVM bitcode library


This is a fork of the [musl libc](http://www.musl-libc.org/).

It is a work in progress.

The idea is to produce a, complete as possible, LLVM bitcode version of
musl libc, motiviated in a simliar fashion to [Klee's uclibc](https://github.com/klee/klee-uclibc).

Our approach here will be to [port musl libc](http://wiki.musl-libc.org/wiki/Porting) to a new
architecture, based on the x86_64 version, but with all assembly replaced by llvm bitcode (maybe).

## Recipe for libc.so.bc
```
WLLVM_CONFIGURE_ONLY=1  CC=wllvm ./configure --target=LLVM --prefix=<install dir>
make
cd lib
extract-bc libc.so
cp libc.so.bc  <wherever you want your bitcode library to live>
```