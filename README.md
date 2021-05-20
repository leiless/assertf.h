# assertf.h

[![Try it online](https://img.shields.io/badge/try-online-d24dff.svg)](https://repl.it/@leiless/assertfh-demo)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5ea731ec4ced42a59cb902012fdfa7b9)](https://www.codacy.com/manual/leiless/assertf.h?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=leiless/assertf.h&amp;utm_campaign=Badge_Grade)
[![License](https://img.shields.io/badge/license-BSD--2--Clause-blue)](LICENSE)

[`assertf.h`](assertf.h) is a header-only formattable assert macros library, a possible alternative to the `#include <assert.h>`.

With enhanced assertions, we can debug/test code better.

## Platform

![Platform](https://img.shields.io/badge/platform-Linux%20%7C%20Windows%20%7C%20macOS%20%7C%20*BSD-e65c00.svg)

`assertf.h` originally targets to embedded systems, it also can be used nearly all UNIX-like, including Windows systems.

## Example

```c
#include <stdio.h>
#include <unistd.h>
#include <errno.h>

#define ASSERTF_DEF_ONCE
#include "assertf.h"

int main(void)
{
    int e = rmdir("/tmp");
    assert_eqf(e, 0, %d, "rmdir(2) fail, errno: %d", errno);    /* Line 11 */
    // #define EACCES          13      /* Permission denied */
    return 0;
}
```

Sample assertion failure output([try it online](https://repl.it/@leiless/assertfh-demo))

![Samplt output](https://user-images.githubusercontent.com/38041294/90534891-dad15d80-e1ac-11ea-8123-48f4b13bad67.png)

## Integration

In order to deploy `assertf.h` to your existing C/C++ project, you need:

1. Download [`assertf.h`](https://raw.githubusercontent.com/leiless/assertf.h/master/assertf.h) to your source code directory

1. In **one** of your C/C++ file(typically project main file), write:
    ```c
    // Define ASSERTF_DEF_ONCE once and only once
    #define ASSERTF_DEF_ONCE
    #include "assertf.h"
    ```

1. If other C/C++ files needs to use `assertf.h`, just type `#include "assertf.h"`.

1. If you want to disable `assertf.h` on release build, please specify `-DASSERTF_DISABLE` in `Makefile`, `CMakeLists.txt`, etc.

## API

Nearly all `assertf.h` APIs prefixed with `assert`, the most basic API is the

* `assertf(expr, fmt, ...)`

    When `expr` isn't true, it'll print out a message backed by `fmt` and `...` to `stderr` and then crash the whole program(if `assertf.h` is enabled).

    Sample output:
    ```c
    // assertf(e == 0, "unlink(2) errno: %d", errno);
    Assert (e == 0) failed: unlink(2) errno: 2 [test.c:12 (main)]
    [1]    62760 abort (core dumped)  ./test
    ```

* `panicf(fmt, ...)`

    Alias call of `assertf(0, fmt, ...)`.

* `assert_nonnull(ptr)`, `assert_null(ptr)`

    Assert nullability of `ptr`.

* `assert_eq(a, b, fs)`, `assert_ne()`, `assert_lt()`, `assert_le()`, `assert_gt()`, `assert_ge()`

    Check arithmetic relation of `a` and `b`, `fs` is the format specifier of `a`, `b` will use the same format specifier as `a`, double-quote in `"%<type_specifier>"` can be omitted.

    ```c
    // assert_eq(e, 0, %d);
    Assert ((e) == (__type0(e)) (0)) failed: lhs: -1 rhs: 0 [test.c:13 (main)]
    [1]    65959 abort (core dumped)  ./test
    ```

* `assert_eqf(a, b, fs, fmt, ...)`, `assert_nef()`, `assert_ltf()`, `assert_lef()`, `assert_gtf()`, `assert_gef()`

    Like above version, `fmt` and `...` can used for verbose assertion output once it failed.

    ```c
    // assert_eqf(e, 0, %d, "unlink(2) errno: %d", errno);
    Assert ((e) == (__type0(e)) (0)) failed: lhs: -1 rhs: 0  unlink(2) errno: 2 [test.c:14 (main)]
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

* Do **NOT** `#define ASSERTF_DISABLE` in any part of your project source code, it'll break compilation semantics of `assertf.h`. Like aforementioned, define it in `Makefile`, `CMakeLists.txt`, etc.

* Like `#include <assert.h>`, all `assertf.h` APIs **isn't** side-effect safe.

* Like `#include <assert.h>`, when the `expr` not true, [`abort(3)`](https://man7.org/linux/man-pages/man3/abort.3.html) will be called eventually.

* Some C/C++ compilers may warns you `implicit declaration of function 'typeof' is invalid in C99 [-Wimplicit-function-declaration]`, in such case, you may replace the `typeof` with `__typeof__` and try again.

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

* Use `assertf.h` heavily and confidently in your code base.

* Replace `#include <assert.h>` with `#include "assertf.h"`. 😌

## FAQ

* HOWTO check if `assertf.h` disabled on a certain build?

    - UNIX-like systems

        ```shell
        $ nm some_binary | grep assertf
        0000000000007709 T x_assertf_c21162d2
        ```

        If you see `x_assertf_c21162d2`, it means `assertf.h` is enabled in `some_binary`.

        Note that if you [`strip(1)`](https://man7.org/linux/man-pages/man1/strip.1.html) the symbols, you have no way to determine whether `assertf.h` disabled or not.

    - Windows

        You need have to check the `pdb`([Program database](https://en.wikipedia.org/wiki/Program_database)) file.
