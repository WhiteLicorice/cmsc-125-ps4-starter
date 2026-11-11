<!--no-pdf-->
# Model Example

This folder holds one worked example of the remote-call model. It is not
graded. It is not part of the work you submit.

| Folder | What it shows |
|:--|:--|
| `rpc` | three fixed failure schedules under the three delivery policies |

Build and check it:

```bash
cd examples/rpc
make
make check
make clean
```

`make` builds the example. `make check` builds it and compares the output with
the recorded output. `make clean` removes the build output. Read the
`README.md` in the folder for the model and the schedules.

Run these commands inside Ubuntu. The example needs a C compiler and `make`.
