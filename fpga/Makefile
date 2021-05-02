mkfile_path := $(abspath $(lastword $(MAKEFILE_LIST)))
current_dir := $(patsubst %/,%,$(dir $(mkfile_path)))
TOP:=Top
VERILOG:=${current_dir}/../GameBrian/generated_output/Top.v
BUILDDIR:=build

BITSTREAM_DEVICE := artix7
PARTNAME:= xc7a100tcsg324-1
DEVICE:= xc7a100t_test
XDC:=${current_dir}/arty.xdc

.DELETE_ON_ERROR:
.PHONY: prog clean

prog: ${BUILDDIR} ${BUILDDIR}/${TOP}.bit
	openocd -f digilent_arty.cfg -c "init; pld load 0 ${BUILDDIR}/Top.bit; exit"

bitstream: ${BUILDDIR} ${BUILDDIR}/${TOP}.bit

${BUILDDIR}:
	mkdir -p ${BUILDDIR}

${BUILDDIR}/${TOP}.eblif: ${VERILOG} ${XDC}
	cd ${BUILDDIR} && symbiflow_synth -t ${TOP} -v ${VERILOG} -d ${BITSTREAM_DEVICE} -p ${PARTNAME} -x ${XDC} 2>&1 > /dev/null

${BUILDDIR}/${TOP}.net: ${BUILDDIR}/${TOP}.eblif
	cd ${BUILDDIR} && symbiflow_pack -e ${TOP}.eblif -d ${DEVICE} 2>&1 > /dev/null

${BUILDDIR}/${TOP}.place: ${BUILDDIR}/${TOP}.net
	cd ${BUILDDIR} && symbiflow_place -e ${TOP}.eblif -d ${DEVICE} -n ${TOP}.net -P ${PARTNAME} 2>&1 > /dev/null

${BUILDDIR}/${TOP}.route: ${BUILDDIR}/${TOP}.place
	cd ${BUILDDIR} && symbiflow_route -e ${TOP}.eblif -d ${DEVICE} 2>&1 > /dev/null

${BUILDDIR}/${TOP}.fasm: ${BUILDDIR}/${TOP}.route
	cd ${BUILDDIR} && symbiflow_write_fasm -e ${TOP}.eblif -d ${DEVICE}

${BUILDDIR}/${TOP}.bit: ${BUILDDIR}/${TOP}.fasm
	cd ${BUILDDIR} && symbiflow_write_bitstream -d ${BITSTREAM_DEVICE} -f ${TOP}.fasm -p ${PARTNAME} -b ${TOP}.bit

clean:
	rm -rf ${BUILDDIR}
