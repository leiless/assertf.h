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

    Example:
    ```shell script
    $ cat test.c
    #include <stdio.h>
    #include <errno.h>
    #include <unistd.h>

    #define ASSERTF_DEF_ONCE
    #include "assertf.h"

    int main(void)
    {
        const char *path = "/tmp/.5a87aed5-df2d-4a12-947d-f2e5792acddc.sock";
        int e = unlink(path);
        assertf(e == 0, "unlink(2) fail  errno: %d", errno);
        return 0;
    }

    $ gcc -Wall -Wextra test.c -otest && ./test
    Assert (e == 0) failed: unlink(2) fail  errno: 2  test.c@main()#12
    [1]    62760 abort (core dumped)  ./test
    ```

* `BUILD_BUG_ON()`

## Caveats

* If you want to disable `assertf.h` on release build, please specify `-DASSERTF_DISABLE` on `Makefile`, `CMakeLists.txt`, etc.

* Do **NOT** `#define ASSERTF_DISABLE` in any part of your source code, it'll break compilation semantics of `assertf.h`. Like aforementioned, define it in `Makefile`, etc.

* Just like `#include <assert.h>`, all `assertf.h` APIs isn't side-effect safe.

* Just like `#include <assert.h>`, when the `expr` not true, `abort(3)` will be called eventually.

## FAQ

* HOWTO check if `assertf.h` disabled on a certain build?

    - UNIX-like systems
    ```shell script
    $ nm some_binary | grep assertf
    0000000000007709 T __assertf0
    ```

    If you see `__assertf0`, it means `assertf.h` is enabled in `some_binary`.
