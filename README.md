A Bela project which loads FAUST code from a `.dsp` file, compiles it on the fly and runs it.
The FAUST code is converted to LLVM IR and then JIT'ed to executable binary on the fly.

Prerequisites:
* `libssl-dev`
* LLVM libraries compiled with `-DLLVM_ENABLE_RTTI=ON` (i.e.: get them with `apt-get` or compile
them yourself: the binaries you would download from the LLVM website would not be suitable).
* compile and install the `faust2` branch of https://github.com/grame-cncm/faust

Usage:
* at build time pass the following flags to `make`: 
```
LDLIBS=-lfaust `llvm-config --ldflags --libs all --system-libs` -ltinfo -lssl -lcrypto
```
* at runtime, pass the name of the `.dsp` file as a command-line argument:
  `--faust faust.dsp`
 
The above options are already set in the `settings.json` and will therefore be used automatically
if you build and run this project from within the Bela IDE.