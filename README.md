<!--no-pdf-->
# CMSC 125 Problem Set 4 Starter

This C17 project is the starter for the CMSC 125 Problem Set 4 remote-call
and remote-file models. A schedule file sets a delivery policy and lists the
messages that arrive, are lost, or are retried. The program prints a trace for
each event. A second model compares two remote file systems under a write and
a later read.

This project runs on Linux, on macOS, and on Windows through WSL2 Ubuntu. It
uses only the C standard library.

## Pair

Enter two names below. An assigned trio adds one entry.

- Full Name (`@github-username`)
- Full Name (`@github-username`)

## Files You May Change

You may change only these files and folders:

- `src/server.c`
- `src/client.c`
- `src/policy.c`
- `src/nfs.c`
- `src/afs.c`
- `ANALYSIS.md`, which you create
- The Pair section of `README.md`
- `student-cases/`, which holds the one case of your own

Everything else is fixed. `include/ps4.h`, `src/main.c`, `src/schedule.c`, and
`src/print.c` are given to you complete. `cases/`, the tools, and the workflow
are the grading corpus and its runner. The manual states every rule these files
follow.

## Layout

```
include/ps4.h         fixed interface: limits, structures, prototypes
src/main.c            complete: the command line and the exit codes
src/schedule.c        complete: the schedule parser
src/print.c           complete: every output line
src/server.c          yours: the server total and the duplicate record
src/client.c          yours: the run loop, the client copies, the retries
src/policy.c          yours: the three repeat rules and the operations
src/nfs.c             yours: the stateless file model
src/afs.c             yours: the whole-file cache model
cases/                the complete public corpus
student-cases/        the one boundary or failure case of your own
tools/check_cases.py  the runner that grades the corpus
tools/check_invariants.py  the five property checks
examples/rpc/         one worked example of the RPC model
ANALYSIS.md           the joint analysis that you create
Makefile run check.sh  the run contract
```

## Commands

```bash
make                      # build build/ps4
make clean                # remove the build directory
./run cases/server/first-apply.case
./check.sh                # build, then run every published case in cases/
./check.sh student-cases  # build, then run the cases in student-cases/
```

`./run` accepts one path to a case file.

`check.sh` is the complete public automated check. No hidden suite exists. Read
`tools/check_cases.py` before you trust a green run. The rubric separately
assesses Implementation Correctness, Structure and Contract, Joint Analysis, and
Collaboration.

## Exit Codes

| Code | Meaning |
|---|---|
| 0 | The case ran and every check in it passed. |
| 1 | A case check failed, or the case content was refused. |
| 2 | The command line or the case file path was refused. |

## The First Run

A fresh clone builds without warnings. It passes 8 of the 58 cases and exits
1. The same run reports 0 of the 5 invariant checks passed. The 8 passing cases
are five parse refusals and three partway failures. The parse refusals need no
module. The partway failures pass because the unfinished `ps4_run` returns
failure without printing. The stubs return an invalid version or leave the
state unchanged.

The other 50 cases need `src/server.c`, `src/client.c`, `src/policy.c`,
`src/nfs.c`, and `src/afs.c`. The workflow in the Actions tab is red for the
same reason. That state is correct for a starter. Your fork passes after you
implement the five files. A complete submission reports `58/58 cases passed`
and `5/5 invariant checks passed`, then exits 0.

Enable workflows in the Actions tab after you fork the repository. GitHub
disables them on forks. The workflow run is part of the submission.

## Hosts

### Linux

```bash
sudo apt update
sudo apt install -y build-essential make python3
```

### macOS

The Xcode command line tools carry the compiler, `make`, and Python 3.

```bash
xcode-select --install
```

Check the Python 3 version. macOS ships Python 3 with the command line tools.

```bash
python3 --version
```

### Windows Through WSL2 Ubuntu

Install WSL2 with Ubuntu from PowerShell.

```powershell
wsl --install -d Ubuntu
```

Open the Ubuntu terminal and install the tools inside it.

```bash
sudo apt update
sudo apt install -y build-essential make python3
```

Work in the Ubuntu file system under your home directory. A checkout under
`/mnt/c` works but it's slower, and file permissions there behave differently.

### Check

```bash
cc --version
make --version
python3 --version
```

## The Corpus

The 58 cases under `cases/` are the cases the instructor grades with. Each
`.case` file holds one schedule. A valid case has an `.out` file with the exact
expected output. A refused case also has `.err` and `.status` files.

The runner compares standard output, standard error, and the exit status of
every case byte for byte. It stops a case after 15 seconds. A valid case
requires exit code zero and empty standard error.

The one case your pair writes lives under `student-cases/`. That folder holds a
short README and an example in the same form. Run it with
`./check.sh student-cases`.

`check.sh` runs the five invariant checks after the public cases. They cover the
lost-request rule, the lost-reply rule, an idempotent retry, the NFS open rule,
and the AFS callback. The checks are part of the same command, so no hidden
suite exists.
