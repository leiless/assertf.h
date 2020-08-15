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
    return 0;
}
