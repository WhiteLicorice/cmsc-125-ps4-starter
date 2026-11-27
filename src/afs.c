#include "ps4.h"

int afs_open(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* See "The Remote File: AFS". */
    /* TODO: Refetch the whole file when the callback is broken. */
    /* TODO: Keep a pending write across that refetch. */
    /* TODO: Return the cached file while the callback holds. */
    return 0;
}

int afs_read(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* See "The Remote File: AFS". */
    /* TODO: Refetch the whole file when the callback is broken. */
    /* TODO: Keep a pending write across that refetch. */
    /* TODO: Serve the cached file while the callback holds. */
    return 0;
}

void afs_write(struct ps4_file *file, int client, int version)
{
    (void)file;
    (void)client;
    (void)version;
    /* See "The Remote File: AFS". */
    /* TODO: Change only the client's local copy. */
    /* TODO: Record the client's dirty version in its cache record. */
}

int afs_close(struct ps4_file *file, int client)
{
    (void)file;
    (void)client;
    /* See "The Remote File: AFS". */
    /* TODO: Store this client's dirty file on the server at the close. */
    /* TODO: Leave the server untouched when the client has no dirty copy. */
    /* TODO: Break every other client's callback after a store. */
    return 0;
}

int afs_mkdir(struct ps4_file *file, int client, const char *name)
{
    (void)file;
    (void)client;
    (void)name;
    /* See "The Remote File: AFS". */
    /* TODO: Answer the same way the NFS model does. */
    return 0;
}

void afs_crash(struct ps4_file *file)
{
    (void)file;
    /* See "The Remote File: AFS". */
    /* TODO: Break every cached copy when the callbacks are lost. */
    /* TODO: Clear each pending write on that crash. */
}
