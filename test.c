/*
 * Created Aug 15, 2020. leiless.
 */

#include <stdio.h>

#define ASSERTF_DEF_ONCE
#include "assertf.h"

int main(void)
{
    assert_eq(1, 1, %d);
    assert_null(NULL);
    assert_nonnull(main);
    assert_true(1, %d);
    assert_true(-1, %d);
    assert_false(0, %d);
    assert_eqf(!1, 0, %d, "foobar");
    return 0;
}
