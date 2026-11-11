#ifndef PS4_H
#define PS4_H

#define PS4_MAX_SEQ 8
#define PS4_MAX_EVENTS 96
#define PS4_MAX_REQUESTS 8
#define PS4_MAX_CLIENTS 4
#define PS4_NAME_MAX 32
#define PS4_ATTR_WINDOW 3

enum ps4_policy {
    PS4_AT_MOST_ONCE,
    PS4_AT_LEAST_ONCE,
    PS4_EXACTLY_ONCE
};

enum ps4_op {
    PS4_OP_ADD,
    PS4_OP_SET
};

enum ps4_file_model {
    PS4_NFS,
    PS4_AFS
};

enum ps4_repeat {
    PS4_REPEAT_SUPPRESS,
    PS4_REPEAT_APPLY,
    PS4_REPEAT_STORED
};

enum ps4_decision {
    PS4_APPLIED,
    PS4_SUPPRESSED,
    PS4_STORED
};

enum ps4_event_kind {
    PS4_EV_SEND,
    PS4_EV_ARRIVE,
    PS4_EV_LOST,
    PS4_EV_RETRY,
    PS4_EV_DELIVER,
    PS4_EV_DROP_REPLY,
    PS4_EV_GIVE_UP,
    PS4_EV_RESET,
    PS4_EV_FILE_START,
    PS4_EV_OPEN,
    PS4_EV_READ,
    PS4_EV_WRITE,
    PS4_EV_CLOSE,
    PS4_EV_MKDIR,
    PS4_EV_TIME,
    PS4_EV_CRASH
};

struct ps4_event {
    enum ps4_event_kind kind;
    enum ps4_op op;
    int seq;
    int arg;
    int client;
    char name[PS4_NAME_MAX];
};

struct ps4_schedule {
    long total;
    enum ps4_policy policy;
    enum ps4_file_model model;
    int has_total;
    int has_policy;
    int has_model;
    struct ps4_event events[PS4_MAX_EVENTS];
    int count;
};

struct ps4_record {
    int used;
    long reply;
};

struct ps4_server {
    long total;
    int applies;
    struct ps4_record records[PS4_MAX_SEQ + 1];
};

struct ps4_request {
    int used;
    int seq;
    enum ps4_op op;
    int arg;
    int copies;
    int outstanding;
    int has_reply;
    long reply;
    int learned;
    long value;
};

struct ps4_client {
    struct ps4_request requests[PS4_MAX_REQUESTS];
    int count;
};

struct ps4_cache {
    int version;
    int fetched;
    int valid;
};

struct ps4_file {
    int disk;
    int pending;
    int clock;
    int mkdir_made;
    char mkdir_name[PS4_NAME_MAX];
    struct ps4_cache cache[PS4_MAX_CLIENTS];
};

/* Given: the schedule parser and the output lines. */
int ps4_parse(const char *path, struct ps4_schedule *out);
void ps4_print_send(int seq, const char *op, int arg);
void ps4_print_arrive(int seq, int copy);
void ps4_print_apply(int seq, int arg, long total, long reply);
void ps4_print_suppress(int seq, long total);
void ps4_print_stored(int seq, long reply, long total);
void ps4_print_lost(int seq, int copy);
void ps4_print_retry(int seq, int copy);
void ps4_print_drop_reply(int seq);
void ps4_print_deliver(int seq, long value);
void ps4_print_give_up(int seq);
void ps4_print_request(int seq, int learned, long value);
void ps4_print_final(int applies, long total, int copies);
void ps4_print_file_start(int version);
void ps4_print_open(int client, int version);
void ps4_print_read(int client, int version);
void ps4_print_write(int client, int version);
void ps4_print_close(int client, int version);
void ps4_print_mkdir(int client, const char *name, int exists);
void ps4_print_crash(int disk);
void ps4_print_file_final(int disk);

/* Yours: the run loop and the models. */
int ps4_run(const struct ps4_schedule *schedule);

void server_reset(struct ps4_server *server, long total);
enum ps4_decision server_deliver(struct ps4_server *server,
                                 enum ps4_policy policy, int seq,
                                 enum ps4_op op, int arg, long *reply);

int policy_parse(const char *name, enum ps4_policy *out);
const char *policy_name(enum ps4_policy policy);
enum ps4_repeat policy_repeat(enum ps4_policy policy);
int op_parse(const char *name, enum ps4_op *out);
const char *op_name(enum ps4_op op);
long op_apply(enum ps4_op op, long total, int arg);

void nfs_reset(struct ps4_file *file, int version);
int nfs_open(struct ps4_file *file, int client);
int nfs_read(struct ps4_file *file, int client);
void nfs_write(struct ps4_file *file, int client, int version);
int nfs_close(struct ps4_file *file, int client);
int nfs_mkdir(struct ps4_file *file, int client, const char *name);
void nfs_crash(struct ps4_file *file);

void afs_reset(struct ps4_file *file, int version);
int afs_open(struct ps4_file *file, int client);
int afs_read(struct ps4_file *file, int client);
void afs_write(struct ps4_file *file, int client, int version);
int afs_close(struct ps4_file *file, int client);
int afs_mkdir(struct ps4_file *file, int client, const char *name);
void afs_crash(struct ps4_file *file);

#endif
