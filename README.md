The version of clang included in Xcode 9.2 appears to miscompile the minimal
test case program linked to below.  When optimizing (-O2 or better) with
debugging information enabled (with -g), the following program triggers a
"double-free" errror from the macOS memory allocator.

https://github.com/b-spencer/xcode9.2-clang-miscompile

Changing the shape of the code in many small ways, or removing any of the
steps in the `broken()` function seems to avoid the bug.  I haven't spent a
lot of time examining the unwind tables or the disassembly, but it seems that
during exception unwinding, the uncaught exception propagating out of
`broken()` ends up trying to free the memory already deallocated when
`std::unique_ptr<Thing> thing` was reassigned on line 22 of `main.cc`.

This bug manifests itself when targeting macOS x86_64 or the iOS x86_64
simulator, at least.

The expected outcome is as follows:
```
$ make clean all && ./prog
construct Thing=0x7fe7a0c002e0
construct Thing=0x7fe7a0c00340
destroy   Thing=0x7fe7a0c002e0
destroy   Thing=0x7fe7a0c00340
Pass
```

The actual outcome is:
```
$ make clean all && ./prog
construct Thing=0x7f86434002e0
construct Thing=0x7f8643400340
destroy   Thing=0x7f86434002e0
destroy   Thing=0x7f8643400340
prog(42802,0x7fffb3c6b3c0) malloc: *** error for object 0x7f86434002e0: pointer being freed was not allocated
*** set a breakpoint in malloc_error_break to debug
Abort trap: 6
```

The program is annotated with comments that mention some of the changes that
can be made that cause the bug to be avoided.  Running UBSan or ASan finds no
problems in the program, but also avoids triggering the bug.  (So does
removing debug info by not passing `-g`, or by limiting debug info to line
number information only.)

The same program works on Xcode 8 as well as with other versions of clang and
various other compilers on other operating systems.

The clang used is the version that comes with Xcode 9.2.  It self-reports its
version as follows:
```
Apple LLVM version 9.0.0 (clang-900.0.39.2)
Target: x86_64-apple-darwin16.7.0
Thread model: posix
InstalledDir: /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/bin
```
