mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))

GAMEBRIAN := ${current_dir}/GameBrian
VERILOG := ${GAMEBRIAN}/generated_output/Top.v

.PHONY: prog clean

prog: ${VERILOG}
	make -C fpga prog

bitstream: ${VERILOG}
	make -C fpga bitstream

verilog: ${VERILOG}

${VERILOG}:
	(cd ${GAMEBRIAN} && sbt run)


clean:
	make -C fpga clean
