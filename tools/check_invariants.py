#!/usr/bin/env python3
"""Check the properties the public case corpus states one case at a time.

`check.sh` runs these checks after the cases. Each one builds a small
schedule, runs the program, and compares the result with the rule the manual
states. A module that hangs fails its check after 15 seconds, and the checks
after it still run.
"""

from pathlib import Path
import os
import subprocess
import sys
import tempfile

INVARIANT_TIMEOUT_SECONDS = 15
POLICIES = ("at-most-once", "at-least-once", "exactly-once")


def run_case(root: Path, text: str):
    """Run one generated schedule. Return the completed process."""
    handle, path = tempfile.mkstemp(suffix=".case")
    try:
        with os.fdopen(handle, "w", encoding="utf-8") as stream:
            stream.write(text)
        return subprocess.run([str(root / "run"), path],
                              capture_output=True, text=True, check=False,
                              timeout=INVARIANT_TIMEOUT_SECONDS)
    finally:
        os.unlink(path)


def final_of(stdout: str):
    """Return (applies, total, copies) from the final line, or None."""
    for line in stdout.splitlines():
        if line.startswith("final applies="):
            fields = dict(part.split("=", 1) for part in line.split()[1:])
            return (int(fields["applies"]), int(fields["total"]),
                    int(fields["copies"]))
    return None


def learned_of(stdout: str, seq: int):
    """Return the learned value for one request as text, or None."""
    prefix = f"request seq={seq} learned="
    for line in stdout.splitlines():
        if line.startswith(prefix):
            return line[len(prefix):]
    return None


def open_versions(stdout: str):
    """Return every version printed by an open line."""
    return [line.split("version=", 1)[1] for line in stdout.splitlines()
            if line.startswith("open ")]


def read_versions(stdout: str):
    """Return every version printed by a read line."""
    return [line.split("version=", 1)[1] for line in stdout.splitlines()
            if line.startswith("read ")]


def check_lost_request(root: Path):
    totals = []
    for policy in POLICIES:
        result = run_case(root, f"""\
total 50
policy {policy}
send 1 add 10
lost 1
retry 1
arrive 1
deliver 1
""")
        if result.returncode != 0:
            return f"{policy}: the run failed with exit {result.returncode}"
        final = final_of(result.stdout)
        if final is None:
            return f"{policy}: the run printed no final line"
        if final[0] != 1 or final[1] != 60:
            return f"{policy}: expected one application at total 60, got {final}"
        totals.append(final[1])
    if len(set(totals)) != 1:
        return f"the three policies ended at different totals: {totals}"
    return ""


def check_lost_reply(root: Path):
    amo = run_case(root, """\
total 40
policy at-most-once
send 1 add 7
arrive 1
drop-reply 1
retry 1
arrive 1
""")
    exo = run_case(root, """\
total 40
policy exactly-once
send 1 add 7
arrive 1
drop-reply 1
retry 1
arrive 1
deliver 1
""")
    for name, result in (("at-most-once", amo), ("exactly-once", exo)):
        if result.returncode != 0:
            return f"{name}: the run failed with exit {result.returncode}"
    first = final_of(amo.stdout)
    second = final_of(exo.stdout)
    if first is None or second is None:
        return "a run printed no final line"
    if first[1] != second[1]:
        return f"the totals differ: {first[1]} and {second[1]}"
    if learned_of(exo.stdout, 1) != "47":
        return f"exactly-once did not learn the retained reply: {exo.stdout}"
    return ""


def check_set_retry(root: Path):
    for policy in POLICIES:
        ending = "give-up 1" if policy == "at-most-once" else "deliver 1"
        result = run_case(root, f"""\
total 100
policy {policy}
send 1 set 10
arrive 1
drop-reply 1
retry 1
arrive 1
{ending}
""")
        if result.returncode != 0:
            return f"{policy}: the run failed with exit {result.returncode}"
        final = final_of(result.stdout)
        if final is None or final[1] != 10:
            return f"{policy}: an idempotent retry changed the total: {final}"
    return ""


def check_nfs_open(root: Path):
    result = run_case(root, """\
model nfs
file start 1
open c3
open c1
write c1 2
close c1
time 2
open c3
""")
    if result.returncode != 0:
        return f"the run failed with exit {result.returncode}"
    versions = open_versions(result.stdout)
    if versions != ["1", "1", "2"]:
        return f"the open versions are {versions}, expected ['1', '1', '2']"
    return ""


def check_afs_callback(root: Path):
    result = run_case(root, """\
model afs
file start 1
open c1
open c2
write c1 2
close c1
read c2
""")
    if result.returncode != 0:
        return f"the run failed with exit {result.returncode}"
    versions = read_versions(result.stdout)
    if versions != ["2"]:
        return f"the read versions are {versions}, expected ['2']"
    return ""


CHECKS = [
    ("a lost request applies once under every policy", check_lost_request),
    ("a lost reply keeps at-most-once and exactly-once at one total",
     check_lost_reply),
    ("an idempotent retry leaves the total at its argument", check_set_retry),
    ("an NFS open ignores the attribute age", check_nfs_open),
    ("an AFS close breaks the other callbacks", check_afs_callback),
]


def main():
    if len(sys.argv) != 2:
        print("usage: check_invariants.py <work-directory>", file=sys.stderr)
        return 2
    root = Path(sys.argv[1]).resolve()
    if not (root / "run").exists():
        print(f"'{root}' holds no run entry point.", file=sys.stderr)
        return 1
    failures = 0
    for title, check in CHECKS:
        try:
            problem = check(root)
        except (OSError, subprocess.SubprocessError) as error:
            problem = f"the check could not run: {error}"
        if problem:
            failures += 1
            print(f"[FAIL] {title}\n       {problem}")
        else:
            print(f"[PASS] {title}")
    print(f"{len(CHECKS) - failures}/{len(CHECKS)} invariant checks passed")
    return 0 if failures == 0 else 1


if __name__ == "__main__":
    sys.exit(main())
