#include "ps4.h"

int ps4_run(const struct ps4_schedule *schedule)
{
    /* See "The Client and Its Copies" and "The Schedule File". */
    /* TODO: Set the server total and the policy before the loop. */
    /* TODO: Clear the client state before the loop. */
    for (int i = 0; i < schedule->count; i++) {
        switch (schedule->events[i].kind) {
        case PS4_EV_SEND:
            /* TODO: Create the request with copy 1 and print the send line. */
            break;
        case PS4_EV_ARRIVE:
            /* TODO: Check for an outstanding request, print the arrival, ask the server, and print the decision. */
            break;
        case PS4_EV_LOST:
            /* TODO: Print the lost line and leave the request outstanding. */
            break;
        case PS4_EV_RETRY:
            /* TODO: Check for an outstanding request, raise the copy count, and print the retry line. */
            break;
        case PS4_EV_DELIVER:
            /* TODO: Check for a pending reply, record the pending reply, and print the deliver line. */
            break;
        case PS4_EV_DROP_REPLY:
            /* TODO: Clear the pending reply and print the drop-reply line. */
            break;
        case PS4_EV_GIVE_UP:
            /* TODO: Clear the outstanding flag and print the give-up line. */
            break;
        case PS4_EV_RESET:
            /* TODO: Reset the server and the client. */
            break;
        case PS4_EV_FILE_START:
            /* TODO: Reset the file model and print the file line. */
            break;
        case PS4_EV_OPEN:
            /* TODO: Call the selected model and print the open line. */
            break;
        case PS4_EV_READ:
            /* TODO: Call the selected model and print the read line. */
            break;
        case PS4_EV_WRITE:
            /* TODO: Call the selected model and print the write line. */
            break;
        case PS4_EV_CLOSE:
            /* TODO: Call the selected model and print the close line. */
            break;
        case PS4_EV_MKDIR:
            /* TODO: Call the selected model and print the mkdir line. */
            break;
        case PS4_EV_TIME:
            /* TODO: Set the model clock. */
            break;
        case PS4_EV_CRASH:
            /* TODO: Crash the selected model and print the crash line. */
            break;
        }
    }
    /* TODO: Print one summary line per request, then one final line. A file case prints only its final line. */
    return 1;
}
