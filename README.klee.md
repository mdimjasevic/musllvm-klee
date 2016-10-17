# Building musl libc for KLEE

## Requirements:

* Beside the build requirements for musl you'll need whole-program-llvm

## Steps:

* `./configure --disable-shared --enable-debug CC={{PATH_TO}}/whole-program-llvm/wllvm`
* `make CC={{PATH_TO}}/whole-program-llvm/wllvm`

* Remove functions we cannot use yet or we have to replace from the built library:
`ar -d musl/lib/libc.a {{FUNCTION/FILE}}`

      ## Following functions are already defined in the POSIX layer
      - readlink.o
      - stat.o
      - fstat.o
      - utimes.o
      - lstat.o
      - malloc.o # Don't provide malloc implementation, we use KLEE ones
      - fcntl.o # we use the implementation of KLEE POSIX layer
      - open.o # we use the implementation of KLEE POSIX layer
      - ioctl.o # we use the implementation inside the KLEE POSIX layer
      - printf.o # Remove this, if printf should be eecuted symbolically as well
      - vprintf.o # Remove this, if vprintf should be eecuted as well
      - read.o
      - close.o
      - lseek.o
      - getdents.o
      - __getdents.o
      - write.o
      # Following functions are assembler only, therefore they don't have LLVM IR
      - lrintl.o
      - sqrt.o
      - sigsetjmp.o
      - fabs.o
      - dlsym.o
      - fmodl.o
      - truncl.o
      - expm1l.o
      - memset.o
      - ceill.o
      - lrint.o
      - restore.o
      - vfork.o
      - log2l.o
      - memmove.o
      - llrintf.o
      - log10l.o
            - expl.o
      - llrintl.o
      - fabsl.o
      - setjmp.o
      - lrintf.o
      - lrintl.o
      - rintl.o
      - fenv.o
      - llrint.o
      - sqrtf.o
      - syscall_cp.o
      - sqrtl.o
      - remainderl.o
      - asinl.o
      - exp2l.o
      - __invtrigl.o
      - log1pl.o
      - floorl.o
      - logl.o
      - memcpy.o
      - fabsf.o
      - atan2l.o
      - tlsdesc.o
      - longjmp.o
      - clone.o
      - atanl.o
      - acosl.o
      - __unmapself.o
      - clone.o
      
* Remove the bitcode library
  `rm musl/lib/libc.bca`
* `{{PATH_TO}}/whole-program-llvm/extract-bc lib/libc.a`

