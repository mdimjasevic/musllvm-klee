#  Musl Libc LLVM bitcode library


This is a fork of the [musl libc](http://www.musl-libc.org/).

It is a work in progress.

The idea is to produce a, complete as possible, LLVM bitcode version of
musl libc, motiviated in a simliar fashion to [Klee's uclibc](https://github.com/klee/klee-uclibc).

Our approach here will be to [port musl libc](http://wiki.musl-libc.org/wiki/Porting) to a new
architecture, based on the x86_64 version, but with all assembly replaced by llvm bitcode (maybe). At the moment `--target=LLVM` behaves more like a `--no-asm`
switch, eliminating all the asm for which there are vanilla C definitions.


## Recipe for libc.so.bc

Install [wllvm.](https://github.com/SRI-CSL/whole-program-llvm.git)
Then:

```
WLLVM_CONFIGURE_ONLY=1  CC=wllvm ./configure --target=LLVM --prefix=<install dir>
make
cd lib
extract-bc -b libc.a
cp libc.a.bc  <wherever you want your bitcode library to live>
```


## Ian's notes on using the bitcode

Suppose you have an application built,  `nweb.bc` say.  
Then you can do:
```
llc -filetype=obj nweb.bc
llc -filetype=obj libc.a.bc
clang -static -nostdlib nweb.o libc.a.o crt1.o libc.a -o nweb
```
and get a working executable. The use of `llc` is optional in principle,
but my `clang` (3.5) crashes on the `libc.a.bc`. You will find
`crt1.o` and  `libc.a` in the same directory you produced `libc.a.bc`.
Of course this is not very interesting unless you have some fun
with the bitcode before this final linking phase.

You can also do things like:

```
llvm-link nweb.bc libc.a.bc -o nweb_app.bc
opt -O3 nweb_app.bc -o nweb_app_o3.bc
llc -filetype=obj nweb_app_o3.bc
clang -static -nostdlib nweb_app.o crt1.o libc.a -o nweb
```
