/*
 * C header-only formattable assert macros.
 *
 * Created May 21, 2020. leiless.
 * XXX: side-effect unsafe.
 *
 * Usage:
 *  #define ASSERTF_DEF_ONCE
 *  #include "assertf.h"
 *
 *  -DASSERTF_DISABLE in Makefile to disable assertf.h
 *
 * Released under BSD-2-Clause license.
 */

#ifndef __ASSERTF_H
#define __ASSERTF_H

/**
 * Compile-time assertion  see: linux/arch/x86/boot/boot.h
 *
 * [sic modified]
 * BUILD_BUG_ON - break compile if a condition is true.
 * @cond: the condition which the compiler should know is false.
 *
 * If you have some code which relies on certain constants being true, or
 * some other compile-time evaluated condition, you should use BUILD_BUG_ON() to
 * detect if someone changes it unexpectedly.
 */
#ifndef BUILD_BUG_ON
#ifdef DEBUG
#define BUILD_BUG_ON(cond)      ((void) sizeof(char[1 - 2 * !!(cond)]))
#else
#define BUILD_BUG_ON(cond)      ((void) (cond))
#endif
#endif

/* Macro taken from macOS/Frameworks/Kernel/sys/cdefs.h */
#ifndef __printflike
#define __printflike(fmtarg, firstvararg) \
        __attribute__((__format__(__printf__, fmtarg, firstvararg)))
#endif

#ifndef ASSERTF_DISABLE

#ifdef __cplusplus
extern "C" {
#endif
extern void __assertf0(int, const char *, ...) __printflike(2, 3);
#ifdef __cplusplus
}
#endif

#ifdef ASSERTF_DEF_ONCE
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>

/**
 * Formatted version of assert()
 *
 * @param expr  Expression to assert with
 * @param fmt   Format string when assertion failed
 * @param ...   Format string arguments
 */
void __assertf0(int expr, const char *fmt, ...)
{
    if (!expr) {
        va_list ap;
        va_start(ap, fmt);
        (void) vfprintf(stderr, fmt, ap);
        va_end(ap);

        abort();
    }
}
#endif

#include <string.h>

/**
 * basename(3) have inconsistent implementation across UNIX-like systems.
 * Besides, Windows doesn't have such API.
 */
const char * __basename0(const char *path)
{
    char *p;
#ifdef _WIN32
    p = strrchr(path, '\\');
#else
    p = strrchr(path, '/');
#endif
    return p != NULL ? p + 1 : path;
}

#define assertf(e, fmt, ...)                                        \
    __assertf0(!!(e), "Assert (%s) failed: " fmt "  %s@%s()#%d\n",  \
                #e, ##__VA_ARGS__, __basename0(__BASE_FILE__), __func__, __LINE__)
#else
#ifdef __cplusplus
extern "C" {
#endif
/* see: https://stackoverflow.com/questions/29117836/attribute-const-vs-attribute-pure-in-gnu-c */
int __vunused(void *, ...) __attribute__((const));
#ifdef __cplusplus
}
#endif

#ifdef ASSERTF_DEF_ONCE
int __vunused(void *arg, ...)
{
    return arg != NULL;
}
#endif

#define assertf(e, fmt, ...)        (void) __vunused((void *) (uintptr_t) (e), fmt, ##__VA_ARGS__)
#endif

#define panicf(fmt, ...)                    \
    do {                                    \
        assertf(0, fmt, ##__VA_ARGS__);     \
        __builtin_unreachable();            \
    } while (0)

#define assert_nonnull(ptr)         assertf(ptr != NULL, "")
#define assert_null(ptr)            assertf(ptr == NULL, "")

/*
 * Taken from https://stackoverflow.com/a/2653351/13600780
 * see: linux/include/linux/stringify.h
 */
#define __xstr0(x...)               #x
#define __xstr(x...)                __xstr0(x)

#define __assert_cmp0(a, b, fs, op)                                             \
    assertf((a) op (typeof(a)) (b), "lhs: " __xstr(fs) " rhs: " __xstr(fs),     \
            (a), (typeof(a)) (b))

/**
 * @param a     Left hand side
 * @param b     Right hand side
 * @param fs    Format specifier
 * @param op    Comparator
 * @param fmt   Additional format string
 * @param ...   Format string arguments
 */
#define __assert_cmp1(a, b, fs, op, fmt, ...)                                           \
    assertf((a) op (typeof(a)) (b), "lhs: " __xstr(fs) " rhs: " __xstr(fs) "  " fmt,    \
            (a), (typeof(a)) (b), ##__VA_ARGS__)

#define assert_eq(a, b, fs)             __assert_cmp0(a, b, fs, ==)
#define assert_eqf(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, ==, fmt, ##__VA_ARGS__)

#define assert_ne(a, b, fs)             __assert_cmp0(a, b, fs, !=)
#define assert_nef(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, !=, fmt, ##__VA_ARGS__)

#define assert_le(a, b, fs)             __assert_cmp0(a, b, fs, <=)
#define assert_lef(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, <=, fmt, ##__VA_ARGS__)

#define assert_ge(a, b, fs)             __assert_cmp0(a, b, fs, >=)
#define assert_gef(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, >=, fmt, ##__VA_ARGS__)

#define assert_lt(a, b, fs)             __assert_cmp0(a, b, fs, <)
#define assert_ltf(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, <, fmt, ##__VA_ARGS__)

#define assert_gt(a, b, fs)             __assert_cmp0(a, b, fs, >)
#define assert_gtf(a, b, fs, fmt, ...)  __assert_cmp1(a, b, fs, >, fmt, ##__VA_ARGS__)

#define assert_true(x, fs)              assert_ne(x, 0, fs)
#define assert_truef(x, fs, fmt, ...)   assert_nef(x, 0, fs, fm, ##__VA_ARGS__)

#define assert_false(x, fs)             assert_eq(x, 0, fs)
#define assert_falsef(x, fs, fmt, ...)  assert_eqf(x, 0, fs, fm, ##__VA_ARGS__)

#define assert_nonzero                  assert_true
#define assert_zero                     assert_false

#endif

