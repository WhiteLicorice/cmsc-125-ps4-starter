#include "ps4.h"

#include <stdio.h>

void ps4_print_send(int seq, const char *op, int arg)
{
    printf("send seq=%d op=%s arg=%d\n", seq, op, arg);
}

void ps4_print_arrive(int seq, int copy)
{
    printf("arrive seq=%d copy=%d\n", seq, copy);
}

void ps4_print_apply(int seq, int arg, long total, long reply)
{
    printf("apply seq=%d arg=%d total=%ld reply=%ld\n", seq, arg, total,
           reply);
}

void ps4_print_suppress(int seq, long total)
{
    printf("suppress seq=%d total=%ld\n", seq, total);
}

void ps4_print_stored(int seq, long reply, long total)
{
    printf("stored seq=%d reply=%ld total=%ld\n", seq, reply, total);
}

void ps4_print_lost(int seq, int copy)
{
    printf("lost seq=%d copy=%d\n", seq, copy);
}

void ps4_print_retry(int seq, int copy)
{
    printf("retry seq=%d copy=%d\n", seq, copy);
}

void ps4_print_drop_reply(int seq)
{
    printf("drop-reply seq=%d\n", seq);
}

void ps4_print_deliver(int seq, long value)
{
    printf("deliver seq=%d value=%ld\n", seq, value);
}

void ps4_print_give_up(int seq)
{
    printf("give-up seq=%d\n", seq);
}

void ps4_print_request(int seq, int learned, long value)
{
    if (learned)
        printf("request seq=%d learned=%ld\n", seq, value);
    else
        printf("request seq=%d learned=unknown\n", seq);
}

void ps4_print_final(int applies, long total, int copies)
{
    printf("final applies=%d total=%ld copies=%d\n", applies, total, copies);
}

void ps4_print_file_start(int version)
{
    printf("file server=%d\n", version);
}

void ps4_print_open(int client, int version)
{
    printf("open c%d version=%d\n", client, version);
}

void ps4_print_read(int client, int version)
{
    printf("read c%d version=%d\n", client, version);
}

void ps4_print_write(int client, int version)
{
    printf("write c%d version=%d\n", client, version);
}

void ps4_print_close(int client, int version)
{
    printf("close c%d version=%d\n", client, version);
}

void ps4_print_mkdir(int client, const char *name, int exists)
{
    printf("mkdir c%d name=%s result=%s\n", client, name,
           exists ? "exists" : "created");
}

void ps4_print_crash(int disk)
{
    printf("crash disk=%d\n", disk);
}

void ps4_print_file_final(int disk)
{
    printf("final server=%d\n", disk);
}
