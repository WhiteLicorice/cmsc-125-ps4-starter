#include "ps4.h"

void server_reset(struct ps4_server *server, long total)
{
    (void)server;
    (void)total;
    /* See "The Server and the Duplicate Record". */
    /* TODO: Store the starting total. */
    /* TODO: Set the applied count to zero. */
    /* TODO: Clear every sequence number in the duplicate record. */
}

enum ps4_decision server_deliver(struct ps4_server *server,
                                 enum ps4_policy policy, int seq,
                                 enum ps4_op op, int arg, long *reply)
{
    (void)server;
    (void)policy;
    (void)seq;
    (void)op;
    (void)arg;
    (void)reply;
    /* See "The Server and the Duplicate Record". */
    /* TODO: Test the duplicate record for this sequence number. */
    /* TODO: Ask the policy what a repeat does. */
    /* TODO: Return without a reply when the policy suppresses the repeat. */
    /* TODO: Return the retained reply when the policy stores it. */
    /* TODO: Apply the operation on a first copy or an applied repeat. */
    /* TODO: Record the sequence number and the reply total. */
    /* TODO: Count the application. */
    return PS4_SUPPRESSED;
}
