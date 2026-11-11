# Student Cases

Add one boundary or failure case of your own. Give it a `.case` file. Add its expected output under `expected/` with the same stem and an `.out` suffix. A failure case also needs a `.status` file and an `.err` file. A valid case needs neither.

Write `.status` as one integer followed by a newline. Write `.err` as the exact standard-error bytes. Keep `.out` empty only when the command prints nothing before it fails. A failure case that prints some lines first records them in `.out`. For a failure case, record the expected nonzero status and error text, so the checker can tell a planned error from a defect.

`example.case` gives a small valid case. Replace it with your own case before submission. Don't change `tools/check_cases.py` or the public cases.

Run `./check.sh student-cases` to check the case. It prints `1/1 cases passed` when the case passes. To show that the case detects a defect, copy the repository, change one rule in `src/`, and run `./check.sh student-cases` in the copy. It prints `0/1 cases passed`, a `FAIL example.case: ...` line, and exits 1. Restore the change in the copy, or delete the copy. Record each command, output, and exit code in `ANALYSIS.md`, with the defect the case detects.
