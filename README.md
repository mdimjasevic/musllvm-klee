#  Musl Libc LLVM bitcode library


This is a fork of the [musl libc](http://www.musl-libc.org/).

It is a work in progress.

The idea is to produce a, complete as possible, LLVM bitcode version of
musl libc, motiviated in a simliar fashion to [Klee's uclibc](https://github.com/klee/klee-uclibc).

Our approach here will be to [port musl libc](http://wiki.musl-libc.org/wiki/Porting) to a new
architecture, based on the x86_64 version, but with all assembly replaced by llvm bitcode (maybe).

## Recipe for libc.so.bc

Install [wllvm.](https://github.com/SRI-CSL/whole-program-llvm.git)
Then:

```
WLLVM_CONFIGURE_ONLY=1  CC=wllvm ./configure --target=LLVM --prefix=<install dir>
make
cd lib
extract-bc libc.so
cp libc.so.bc  <wherever you want your bitcode library to live>
```


## Ian's notes on using the bitcode

Suppose you have an application built,  `nweb.bc` say.  Build
the `libc.so.bc` via the above with the additional tweak/hack:
```
CFLAGS="-DSRI_TLS_HACK"
```
then you can do:
```
llc -filetype=obj nweb.bc
llc -filetype=obj libc.so.bc
clang -static -notstdlib nweb.o libc.so.o crt1.o libc.a -o nweb
```
and get a working executable. The use of `llc` is optional in principle,
but my `clang` (3.5) crashes on the `libc.so.bc`. You will find
`crt1.o` and  `libc.a` in the same directory you found the `libc.so`.
Of course this is not very interesting unless you have some fun
with the bitcode before the final linking.

You can also do things like:

```
llvm-link nweb.bc libc.so.bc -o nweb_app.bc
opt -O3 nweb_app.bc
llc -filetype=obj nweb_app.bc
clang -static -notstdlib nweb.o libc.so.o crt1.o libc.a -o nweb
```

Blame @ianamason for this recipe.



