A Bela project which loads FAUST code from a `.dsp` file, compiles it on the fly and runs it.
The FAUST code is converted to LLVM IR and then JIT'ed to executable binary on the fly.
This will only work on a Bela image v0.3 or above.

Prerequisites:
tl; dr:
```
apt-get install libssl-dev llvm-3.9-dev libmicrohttp-dev
update-alternatives --install /usr/bin/llvm-config llvm-config /usr/bin/llvm-config-3.9 1
git clone https://github.com/grame-cncm/faust.git && git submodule update --init && cd faust && make all && make install
```
Long version:
* connect Bela to the internet (e.g.: https://github.com/BelaPlatform/Bela/wiki/Connecting-Bela-to-wifi)
* install `libssl-dev` and `libmicrohttp-dev`
* install LLVM libraries compiled with `-DLLVM_ENABLE_RTTI=ON` (i.e.: get them with `apt-get` or compile
them yourself: the binaries you would download from the LLVM website would not be suitable).
* bonus step: get a `distcc` setup working to speed up compilation (e.g.: see [here](https://github.com/BelaPlatform/bela_newBlog/blob/master/docs/_drafts/2017-10-31-cross-compiling.markdown and then `export CXX="distcc /usr/local/bin/clang++-3.9-arm"`).
* compile and install the `master` branch of https://github.com/grame-cncm/faust

Usage:
* clone this repo in a Bela project (e.g.: `/root/Bela/project/bela-faust0-jit`)
* at build time pass the following flags to `make`: 
```
LDLIBS="-lfaust `llvm-config --ldflags --libs all --system-libs` -ltinfo -lssl -lcrypto"
```
(note if you put the above in the Bela IDE, omit the " ")
* at runtime, pass the name of the `.dsp` file as a command-line argument:
  `--faust faust.dsp`
 
The above options are already set in the `settings.json` and will therefore be used automatically
if you build and run this project from within the Bela IDE.
