#include "ps4.h"

#include <stdio.h>

int main(int argc, char **argv)
{
    struct ps4_schedule schedule;
    int status;

    if (argc != 2) {
        fputs("usage: run <case-file>\n", stderr);
        return 2;
    }
    status = ps4_parse(argv[1], &schedule);
    if (status != 0) {
        if (status == 2)
            return 2;
        fputs("ps4: case failed\n", stderr);
        return 1;
    }
    if (ps4_run(&schedule) != 0) {
        fputs("ps4: case failed\n", stderr);
        return 1;
    }
    return 0;
}
