# assertf.h

[![](https://img.shields.io/github/license/leiless/assertf.h)](https://img.shields.io/github/license/leiless/assertf.h)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5ea731ec4ced42a59cb902012fdfa7b9)](https://www.codacy.com/manual/leiless/assertf.h?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=leiless/assertf.h&amp;utm_campaign=Badge_Grade)

`assertf.h` is a formattable assert macros library, a possible alternative to the `#include <assert.h>`.

With enhanced assertions, we can debug/test code better.

## Platform

Linux | macOS | *BSD | Windows
--- | --- | --- | ---
Ready | Ready | Ready | N/A

`assertf.h` originally targets to embedded systems, it can be used nearly all UNIX-like systems.

## Integration

In order to deploy `assertf.h` to your existing C/C++ project, you need:

1. Copy `assertf.h` to source code directory
1. In **one** of your C/C++ file(typically project main file), write:
    ```c
    #define ASSERTF_DEF_ONCE
    #include "assertf.h"
    ```
1. If other C/C++ files needs to use `assertf.h`, just type `#include "assertf.h"`.

## API

Nearly all `assertf.h` APIs prefixed with `assert`, the most basic API is the

* `assertf(expr, fmt, ...)`

    When `expr` isn't true, it'll print out a message backed by `fmt` and `...` to `stderr` and then crash the whole program(if `assertf.h` is enabled).

    Sample output:
    ```
    // assertf(e == 0, "unlink(2) fail  errno: %d", errno);
    Assert (e == 0) failed: unlink(2) fail  errno: 2  test.c@main()#12
    [1]    62760 abort (core dumped)  ./test
    ```

* `panicf(fmt, ...)`

    Alias call of `assertf(0, fmt, ...)`.

* `assert_nonnull(ptr)`, `assert_null(ptr)`

    Assert nullability of `ptr`.

* `assert_eq(a, b, fs)`, `assert_ne()`, `assert_lt()`, `assert_le()`, `assert_gt()`, `assert_ge()`

    Check arithmetic relation of `a` and `b`, `fs` is the format specifier of `a`, `b` will use the same format specifier as `a`, double-quote in `"%<type_specifier>"` can be omitted.

    ```
    // assert_eq(e, 0, %d);
    Assert ((e) == (typeof(e)) (0)) failed: lhs: -1 rhs: 0  test.c@main()#13
    [1]    65959 abort (core dumped)  ./test
    ```

* `assert_eqf(a, b, fs, fmt, ...)`, `assert_nef()`, `assert_ltf()`, `assert_lef()`, `assert_gtf()`, `assert_gef()`

    Like above version, `fmt` and `...` can used for verbose assertion output once it failed.

    ```
    // assert_eqf(e, 0, %d, "unlink(2) fail  errno: %d", errno);
    Assert ((e) == (typeof(e)) (0)) failed: lhs: -1 rhs: 0  unlink(2) fail  errno: 2  test.c@main()#14
    [1]    66800 abort (core dumped)  ./test
    ```

* `assert_true(x, fs)`, `assert_truef(x, fs, fmt, ...)`, `assert_false()`, `assert_falsef()`

    Alias call of `assert_ne(x, 0, fs, ...)`, `assert_eq(x, 0, fs, ...)`.

* `assert_nonzero(x, fs)`, `assert_zero(x, fs)`

    Alias call of `assert_true(x, fs)`, `assert_false(x, fs)`.

* `BUILD_BUG_ON()`

    Break compile if a condition is true at compile-time, taken from Linux kernel. It's useful with companion of `assert*`.

    If you have some code which relies on certain constants being true, or some other compile-time evaluated condition, you should use `BUILD_BUG_ON()` to detect if someone changes it unexpectedly.

## Caveats

* If you want to disable `assertf.h` on release build, please specify `-DASSERTF_DISABLE` on `Makefile`, `CMakeLists.txt`, etc.

* Do **NOT** `#define ASSERTF_DISABLE` in any part of your project source code, it'll break compilation semantics of `assertf.h`. Like aforementioned, define it in `Makefile`, etc.

* Like `#include <assert.h>`, all `assertf.h` APIs **isn't** side-effect safe.

* Like `#include <assert.h>`, when the `expr` not true, `abort(3)` will be called eventually.

## Pro tips

* Do **NOT** misuse any assertion library, assertion is very useful in software development, many people rely on it heavily and some certainly misused it.

    Use assertion to assure code quality isn't a decent way to solve the problem.

    Most software have a very long life-time, sometimes keep running is better than simply crashed.

    SEE ALSO: [Why does Go not have assertions?](https://golang.org/doc/faq#assertions)

* Do **NOT** write side-effect unsafe code:

    ```c
    // When you -DASSERTF_DISABLE, lseek(2) may optimized out by the compiler.
    assert_gt(lseek(fd, offset, SEEK_CUR), 0, %d);
    ```

    ```c
    // The ++i will be evaluated twice when expanding assert_eq() macro
    // Again, -DASSERTF_DISABLE may cause ++i optimized out by the compiler
    assert_eq(++i, n, %d);
    ```

* Use `assertf.h` heavily in your test code.

* Replace `#include <assert.h>` with `#include "assertf.h"`. 😄

## FAQ

* HOWTO check if `assertf.h` disabled on a certain build?

    - UNIX-like systems
    ```shell script
    $ nm some_binary | grep assertf
    0000000000007709 T __assertf0
    ```

    If you see `__assertf0`, it means `assertf.h` is enabled in `some_binary`.

