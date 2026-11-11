#include "ps4.h"

enum ps4_repeat policy_repeat(enum ps4_policy policy)
{
    (void)policy;
    /* TODO: Return suppression for at-most-once. */
    /* TODO: Return application for at-least-once. */
    /* TODO: Return the retained reply for exactly-once. */
    return PS4_REPEAT_SUPPRESS;
}

long op_apply(enum ps4_op op, long total, int arg)
{
    (void)op;
    (void)total;
    (void)arg;
    /* TODO: Add the argument for an add operation. */
    /* TODO: Replace the total with the argument for a set operation. */
    return total;
}
