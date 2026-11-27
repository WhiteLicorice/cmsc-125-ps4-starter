#!/usr/bin/env sh
set -eu
root=$(CDPATH= cd -- "$(dirname -- "$0")" && pwd)
make -C "$root"
cd "$root"
case_dir=${1:-cases}
case_path=$(CDPATH= cd -- "$case_dir" 2>/dev/null && pwd) || case_path="$root/$case_dir"
case_count=$(find "$case_path" -name '*.case' | wc -l | tr -d ' ')
echo "Checking $case_count cases in $case_dir."
status=0
python3 tools/check_cases.py "$case_dir" || status=$?
if [ "$case_path" = "$root/cases" ]; then
    echo
    echo "== invariants =="
    python3 tools/check_invariants.py "$root" || status=$?
fi
exit $status
