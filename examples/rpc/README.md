<!--no-pdf-->
# RPC Example

This program runs three fixed failure schedules under the three delivery policies. Every result is deterministic.

The program prints three schedules with nine result lines. Each schedule prints a header line first. A result line gives the policy, the number of times the server applied a request, the server total, and what the client learns.

Schedule 1 loses one reply and sends the retry. Schedule 2 sends request 2 before the retry of request 1. Schedule 3 loses the first request, so the retry is the first copy the server sees.

The check saves the program's complete output to `actual.txt` and compares that file with the checked-in expected file. That file records the result of the stated model.

These examples are not graded. They are not part of the work you submit. Use them to see the model before you write the code.

Run commands in this directory. On macOS, use a terminal. The Xcode command line tools carry the compiler and `make`. On Windows, use WSL2 Ubuntu. On Linux, use a terminal. You need a C compiler and `make`.

Build with `make`. Then run `./example` to read the three schedules. `make check` saves the program's output to `actual.txt` and prints only the comparison result. Remove generated files with `make clean`.
