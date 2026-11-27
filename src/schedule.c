#include "ps4.h"

#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_TOKENS 5

int policy_parse(const char *name, enum ps4_policy *out)
{
    if (strcmp(name, "at-most-once") == 0)
        *out = PS4_AT_MOST_ONCE;
    else if (strcmp(name, "at-least-once") == 0)
        *out = PS4_AT_LEAST_ONCE;
    else if (strcmp(name, "exactly-once") == 0)
        *out = PS4_EXACTLY_ONCE;
    else
        return -1;
    return 0;
}

const char *policy_name(enum ps4_policy policy)
{
    if (policy == PS4_AT_MOST_ONCE)
        return "at-most-once";
    if (policy == PS4_AT_LEAST_ONCE)
        return "at-least-once";
    return "exactly-once";
}

int op_parse(const char *name, enum ps4_op *out)
{
    if (strcmp(name, "add") == 0)
        *out = PS4_OP_ADD;
    else if (strcmp(name, "set") == 0)
        *out = PS4_OP_SET;
    else
        return -1;
    return 0;
}

const char *op_name(enum ps4_op op)
{
    return op == PS4_OP_ADD ? "add" : "set";
}

static int parse_long(const char *text, long *out)
{
    char *end;
    long value;

    errno = 0;
    value = strtol(text, &end, 10);
    if (errno != 0 || end == text || *end != '\0')
        return -1;
    *out = value;
    return 0;
}

static int parse_int(const char *text, int *out)
{
    long value;

    if (parse_long(text, &value) != 0 || value < INT_MIN || value > INT_MAX)
        return -1;
    *out = (int)value;
    return 0;
}

static int split(char *line, char *token[], int limit)
{
    int count = 0;
    char *cursor = line;

    while (*cursor != '\0') {
        while (*cursor == ' ' || *cursor == '\t' || *cursor == '\r' ||
               *cursor == '\n')
            cursor++;
        if (*cursor == '\0')
            break;
        if (count == limit)
            return -1;
        token[count++] = cursor;
        while (*cursor != '\0' && *cursor != ' ' && *cursor != '\t' &&
               *cursor != '\r' && *cursor != '\n')
            cursor++;
        if (*cursor != '\0')
            *cursor++ = '\0';
    }
    return count;
}

static int add_event(struct ps4_schedule *out, enum ps4_event_kind kind)
{
    struct ps4_event *event;

    if (out->count == PS4_MAX_EVENTS)
        return -1;
    event = &out->events[out->count++];
    memset(event, 0, sizeof *event);
    event->kind = kind;
    return 0;
}

static int check_seq(const char *text, int *out)
{
    int seq;

    if (parse_int(text, &seq) != 0 || seq < 1 || seq > PS4_MAX_SEQ)
        return -1;
    *out = seq;
    return 0;
}

static int check_client(const char *text, int *out)
{
    int client;

    if (text[0] == 'c' || text[0] == 'C')
        text++;
    if (parse_int(text, &client) != 0 || client < 1 ||
        client > PS4_MAX_CLIENTS)
        return -1;
    *out = client;
    return 0;
}

static int read_line(FILE *input, struct ps4_schedule *out, int *line_number)
{
    char line[256];
    char *token[MAX_TOKENS];
    int tokens;
    struct ps4_event *event;

    if (fgets(line, sizeof line, input) == NULL)
        return 1;
    (*line_number)++;
    if (strchr(line, '#') != NULL)
        *strchr(line, '#') = '\0';
    tokens = split(line, token, MAX_TOKENS);
    if (tokens < 0)
        return -1;
    if (tokens == 0)
        return 0;

    if (strcmp(token[0], "total") == 0) {
        if (tokens != 2 || out->has_total)
            return -1;
        if (parse_long(token[1], &out->total) != 0)
            return -1;
        out->has_total = 1;
        return 0;
    }
    if (strcmp(token[0], "policy") == 0) {
        if (tokens != 2 || out->has_policy)
            return -1;
        if (policy_parse(token[1], &out->policy) != 0)
            return -1;
        out->has_policy = 1;
        return 0;
    }
    if (strcmp(token[0], "model") == 0) {
        if (tokens != 2 || out->has_model)
            return -1;
        if (strcmp(token[1], "nfs") == 0)
            out->model = PS4_NFS;
        else if (strcmp(token[1], "afs") == 0)
            out->model = PS4_AFS;
        else
            return -1;
        out->has_model = 1;
        return 0;
    }
    if (strcmp(token[0], "send") == 0) {
        if (tokens != 4 || add_event(out, PS4_EV_SEND) != 0)
            return -1;
        event = &out->events[out->count - 1];
        if (check_seq(token[1], &event->seq) != 0)
            return -1;
        if (op_parse(token[2], &event->op) != 0)
            return -1;
        return parse_int(token[3], &event->arg);
    }
    if (strcmp(token[0], "arrive") == 0 || strcmp(token[0], "lost") == 0 ||
        strcmp(token[0], "retry") == 0 || strcmp(token[0], "deliver") == 0 ||
        strcmp(token[0], "drop-reply") == 0 ||
        strcmp(token[0], "give-up") == 0) {
        enum ps4_event_kind kind;

        if (tokens != 2)
            return -1;
        if (strcmp(token[0], "arrive") == 0)
            kind = PS4_EV_ARRIVE;
        else if (strcmp(token[0], "lost") == 0)
            kind = PS4_EV_LOST;
        else if (strcmp(token[0], "retry") == 0)
            kind = PS4_EV_RETRY;
        else if (strcmp(token[0], "deliver") == 0)
            kind = PS4_EV_DELIVER;
        else if (strcmp(token[0], "drop-reply") == 0)
            kind = PS4_EV_DROP_REPLY;
        else
            kind = PS4_EV_GIVE_UP;
        if (add_event(out, kind) != 0)
            return -1;
        return check_seq(token[1], &out->events[out->count - 1].seq);
    }
    if (strcmp(token[0], "reset") == 0) {
        if (tokens != 2 || add_event(out, PS4_EV_RESET) != 0)
            return -1;
        return parse_int(token[1], &out->events[out->count - 1].arg);
    }
    if (strcmp(token[0], "file") == 0) {
        int version;

        if (tokens != 3 || strcmp(token[1], "start") != 0 ||
            add_event(out, PS4_EV_FILE_START) != 0)
            return -1;
        if (parse_int(token[2], &version) != 0 || version < 1)
            return -1;
        out->events[out->count - 1].arg = version;
        return 0;
    }
    if (strcmp(token[0], "open") == 0 || strcmp(token[0], "read") == 0 ||
        strcmp(token[0], "close") == 0) {
        enum ps4_event_kind kind;

        if (tokens != 2)
            return -1;
        if (strcmp(token[0], "open") == 0)
            kind = PS4_EV_OPEN;
        else if (strcmp(token[0], "read") == 0)
            kind = PS4_EV_READ;
        else
            kind = PS4_EV_CLOSE;
        if (add_event(out, kind) != 0)
            return -1;
        return check_client(token[1], &out->events[out->count - 1].client);
    }
    if (strcmp(token[0], "write") == 0) {
        if (tokens != 3 || add_event(out, PS4_EV_WRITE) != 0)
            return -1;
        event = &out->events[out->count - 1];
        if (check_client(token[1], &event->client) != 0)
            return -1;
        if (parse_int(token[2], &event->arg) != 0 || event->arg < 1)
            return -1;
        return 0;
    }
    if (strcmp(token[0], "mkdir") == 0) {
        if (tokens != 3 || add_event(out, PS4_EV_MKDIR) != 0)
            return -1;
        event = &out->events[out->count - 1];
        if (check_client(token[1], &event->client) != 0)
            return -1;
        if (strlen(token[2]) == 0 || strlen(token[2]) >= PS4_NAME_MAX)
            return -1;
        strcpy(event->name, token[2]);
        return 0;
    }
    if (strcmp(token[0], "time") == 0) {
        if (tokens != 2 || add_event(out, PS4_EV_TIME) != 0)
            return -1;
        if (parse_int(token[1], &out->events[out->count - 1].arg) != 0)
            return -1;
        return out->events[out->count - 1].arg < 0 ? -1 : 0;
    }
    if (strcmp(token[0], "crash") == 0) {
        if (tokens != 1)
            return -1;
        return add_event(out, PS4_EV_CRASH);
    }
    return -1;
}

int ps4_parse(const char *path, struct ps4_schedule *out)
{
    FILE *input;
    int line_number = 0;
    int status;
    int i;
    int dir_seen = 0;
    char dir_name[PS4_NAME_MAX];

    memset(out, 0, sizeof *out);
    out->policy = PS4_AT_MOST_ONCE;
    out->model = PS4_NFS;
    input = fopen(path, "r");
    if (input == NULL) {
        perror("ps4: cannot open case");
        return 2;
    }
    while ((status = read_line(input, out, &line_number)) == 0)
        ;
    fclose(input);
    if (status != 1)
        return 1;
    if (out->has_model) {
        if (out->has_total || out->has_policy)
            return 1;
        for (i = 0; i < out->count; i++) {
            if (out->events[i].kind != PS4_EV_MKDIR)
                continue;
            if (!dir_seen) {
                strcpy(dir_name, out->events[i].name);
                dir_seen = 1;
            } else if (strcmp(out->events[i].name, dir_name) != 0) {
                return 1;
            }
        }
        for (i = 0; i < out->count; i++)
            if (out->events[i].kind < PS4_EV_FILE_START)
                return 1;
        if (out->count == 0 || out->events[0].kind != PS4_EV_FILE_START)
            return 1;
        return 0;
    }
    if (!out->has_total || !out->has_policy)
        return 1;
    for (i = 0; i < out->count; i++)
        if (out->events[i].kind >= PS4_EV_FILE_START)
            return 1;
    return 0;
}
