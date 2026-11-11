#include "ps4.h"

int nfs_open(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* TODO: Fetch the server's version at the open. */
    /* TODO: Record when the client fetched it. */
    /* TODO: Mark the cached attributes valid. */
    return 0;
}

int nfs_read(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* TODO: Fetch the block when the cache is invalid. */
    /* TODO: Fetch the block when the attributes have expired. */
    /* TODO: Serve the cached version inside the window. */
    return 0;
}

void nfs_write(struct ps4_file *file, int client, int version)
{
    (void)file;
    (void)client;
    (void)version;
    /* TODO: Store the new version in the client's cached copy. */
    /* TODO: Hold the version until the close makes it durable. */
}

int nfs_close(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* TODO: Store a pending write on the server at the close. */
    /* TODO: Refresh the client's cached version and attributes. */
    return 0;
}

int nfs_mkdir(struct ps4_file *file, int client, const char *name)
{
    (void)file;
    (void)client;
    (void)name;
    /* TODO: Report that the directory exists on a repeated request. */
    /* TODO: Record the new directory name on the first request. */
    return 0;
}

void nfs_crash(struct ps4_file *file)
{
    (void)file;
    /* TODO: Drop a write that never became durable. */
}
