#include <stdio.h>

/* The three delivery policies modelled by this program. */
enum policy {
    AT_MOST_ONCE,
    AT_LEAST_ONCE,
    EXACTLY_ONCE
};

#define MAX_SEQ 8

struct server {
    int total;
    int applies;
    int seen[MAX_SEQ + 1];
    int reply[MAX_SEQ + 1];
};

static void reset(struct server *s, int total)
{
    int i;

    s->total = total;
    s->applies = 0;
    for (i = 0; i <= MAX_SEQ; i++) {
        s->seen[i] = 0;
        s->reply[i] = 0;
    }
}

/*
 * Deliver one request. Return 1 when the server sends a reply, and store
 * the reply value in *out. Return 0 when the server sends nothing, which
 * models a suppressed duplicate under at-most-once.
 */
static int deliver(struct server *s, enum policy p, int seq, int arg, int *out)
{
    if (s->seen[seq]) {
        if (p == AT_MOST_ONCE) {
            return 0;
        }
        if (p == EXACTLY_ONCE) {
            *out = s->reply[seq];
            return 1;
        }
    }

    s->total += arg;
    s->applies++;
    s->seen[seq] = 1;
    s->reply[seq] = s->total;
    *out = s->total;
    return 1;
}

static const char *policy_name(enum policy p)
{
    if (p == AT_MOST_ONCE) {
        return "at-most-once";
    }
    if (p == AT_LEAST_ONCE) {
        return "at-least-once";
    }
    return "exactly-once";
}

/*
 * Schedule 1. The reply to request 1 is lost, so the client retries it.
 * The retry arrives with no later request in between.
 */
static void schedule_one(enum policy p)
{
    struct server s;
    int reply;
    int got;

    reset(&s, 100);
    got = deliver(&s, p, 1, 10, &reply);
    if (!got) {
        printf("unexpected missing reply\n");
    }
    /* The reply is lost in the network. */
    got = deliver(&s, p, 1, 10, &reply);

    printf("%s: applies %d, total %d, client ",
           policy_name(p), s.applies, s.total);
    if (got) {
        printf("%d\n", reply);
    } else {
        printf("unknown\n");
    }
}

/*
 * Schedule 2. The reply to request 1 is lost. The client sends request 2,
 * receives its reply, then retries request 1. The late retry is the point.
 */
static void schedule_two(enum policy p)
{
    struct server s;
    int first = -1;
    int second = -1;
    int got_first;
    int reply;

    reset(&s, 100);
    if (!deliver(&s, p, 1, 10, &reply)) {
        printf("unexpected missing reply\n");
    }
    /* Reply to request 1 is lost. */

    if (!deliver(&s, p, 2, 5, &second)) {
        printf("unexpected missing reply\n");
    }
    /* Reply to request 2 arrives. */

    got_first = deliver(&s, p, 1, 10, &reply);
    if (got_first) {
        first = reply;
    }

    printf("%s: applies %d, total %d, seq 1 ",
           policy_name(p), s.applies, s.total);
    if (got_first) {
        printf("%d", first);
    } else {
        printf("unknown");
    }
    printf(", seq 2 %d\n", second);
}

/*
 * Schedule 3. Request 1 is lost before the server sees it. The retry is the
 * first copy to arrive, so no policy suppresses it.
 */
static void schedule_three(enum policy p)
{
    struct server s;
    int reply;
    int got;

    reset(&s, 50);
    /* Request 1 is lost in the network. The client retries it. */
    got = deliver(&s, p, 1, 10, &reply);

    printf("%s: applies %d, total %d, client ",
           policy_name(p), s.applies, s.total);
    if (got) {
        printf("%d\n", reply);
    } else {
        printf("unknown\n");
    }
}

int main(void)
{
    enum policy policies[] = { AT_MOST_ONCE, AT_LEAST_ONCE, EXACTLY_ONCE };
    int i;

    puts("schedule 1: total 100, add 10 at seq 1, reply to seq 1 lost");
    for (i = 0; i < 3; i++) {
        schedule_one(policies[i]);
    }

    puts("schedule 2: total 100, add 10 at seq 1, add 5 at seq 2, reply to seq 1 lost, retry seq 1 last");
    for (i = 0; i < 3; i++) {
        schedule_two(policies[i]);
    }

    puts("schedule 3: total 50, add 10 at seq 1, request to seq 1 lost");
    for (i = 0; i < 3; i++) {
        schedule_three(policies[i]);
    }

    return 0;
}
