Build script for GameBrian on Arty A7 100T

Now uses fusesoc+vivado since Chisel3 by itself can't deal with bidirectional/tristates too well,
and I need some primitives like IOBUF.  

Unsure if symbiflow will handle it well either.

Chisel will generate a `Top` module, but the real top (`FPGATop`) has IOBUFs and connects it to the generated `Top` module

Requires fusesoc and vivado to be installed and set up

```bash
git clone https://github.com/awkwardbunny/GameBrian-FPGA
git submodule update --recursive
make
```

## Old

Requires symbiflow to be installed and set up.

```bash
git clone https://github.com/awkwardbunny/GameBrian-FPGA
git submodule update --recursive
conda activate xc7
make
```
