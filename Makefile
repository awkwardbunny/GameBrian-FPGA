mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))

GAMEBRIAN := ${current_dir}/GameBrian
FPGA_FUSESOC := ${current_dir}/fpga-fusesoc

CHISEL_VERILOG := ${GAMEBRIAN}/generated_output/Top.v

.PHONY: prog clean sim ramsim
prog: bitstream

bitstream: ${CHISEL_VERILOG}
	fusesoc --cores-root ${FPGA_FUSESOC} run meirlabs::gamebrian

#.PHONY: ${CHISEL_VERILOG}
${CHISEL_VERILOG}:
	(cd ${GAMEBRIAN} && sbt run)

ramsim: sim/ram_tb.v
	fusesoc --cores-root ${FPGA_FUSESOC} run --target=$@ meirlabs::gamebrian
	#xdg-open build/meirlabs__gamebrian_0.1/$@-icarus/dump.vcd

sim: sim/top_tb.v
	fusesoc --cores-root ${FPGA_FUSESOC} run --target=$@ meirlabs::gamebrian
	#xdg-open build/meirlabs__gamebrian_0.1/$@-icarus/dump.vcd

clean:
	rm -rf build ${GAMEBRIAN}/generated_output
