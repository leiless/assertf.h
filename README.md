# assertf.h

[![](https://img.shields.io/github/license/leiless/assertf.h)](https://img.shields.io/github/license/leiless/assertf.h)
[![Codacy Badge](https://app.codacy.com/project/badge/Grade/5ea731ec4ced42a59cb902012fdfa7b9)](https://www.codacy.com/manual/leiless/assertf.h?utm_source=github.com&amp;utm_medium=referral&amp;utm_content=leiless/assertf.h&amp;utm_campaign=Badge_Grade)

`assertf.h` is a formattable assert macros library, a possible alternative to the `#include <assert.h>`.

With enhanced assertions, we can debug/test code better.

## Platform

Linux | macOS | *BSD | Windows
--- | --- | --- | ---
Ready | Ready | Ready | N/A

## Integration

In order to deploy `assertf.h` to your existing C/C++ project, you need:

1. Copy `assertf.h` to source code directory
1. In **one** of your C/C++ file(typically project main file), write:
    ```c
    #define ASSERTF_IMPLEMENTATION
    #include "assertf.h"
    ```
1. If other C/C++ files needs to use `assertf.h`, just type `#include "assertf.h"`.

## API

TODO

## Caveats

* If you want to disable `assertf.h` on release build, please specify `-DASSERTF_DISABLE` on `Makefile`, `CMakeLists.txt`, etc.

* Do **NOT** `#define ASSERTF_DISABLE` in any place of the source code, it'll break compilation semantics of `assertf.h`. Like aforementioned, define it in `Makefile`, etc.

## FAQ

* HOWTO check if `assertf.h` disabled on a certain build?

    - UNIX-like systems
    ```shell script
    $ nm some_binary | grep assertf
    0000000000007709 T __assertf0
    ```

    If you see `__assertf0`, it means `assertf.h` is enabled in `some_binary`.
