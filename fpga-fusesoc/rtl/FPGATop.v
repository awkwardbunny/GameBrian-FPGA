`timescale 1ns / 1ps

`define RAM_TEST

module FPGATop (
	input wire sys_clk,
	input wire sys_rst,

	input wire uart_rx,
	output wire uart_tx,
	//output wire [7:0] debug,

	//input wire [3:0] btn,
	output wire [3:0] led,

	input wire GBA_CLK,
	input wire GBA_nWR,
	input wire GBA_nRD,
	input wire GBA_nCS,
	inout wire [15:0] GBA_AD,
	inout wire [7:0] GBA_A,
	input wire GBA_nCS2,
	output wire GBA_nREQ,
	input wire GBA_VDD
);

	assign uart_tx = uart_rx;

	// GBA IO Buffers
	wire [15:0] GBA_AD_in;
	wire [15:0] GBA_AD_out;
	wire GBA_AD_oe;
	
	wire [7:0] GBA_A_in;
	wire [7:0] GBA_A_out;
	wire GBA_A_oe;
	
	IOBUF ad_buf[15:0] (
		.IO(GBA_AD), // The pin
		.O(GBA_AD_in), // Output of the buffer, which is input for us (see Xilinx UG953 pg199)
		.I(GBA_AD_out),
		.T(!GBA_AD_oe)
	);

	IOBUF a_buf[7:0] (
		.IO(GBA_A), // The pin
		.O(GBA_A_in), // Output of the buffer, which is input for us (see Xilinx UG953 pg199)
		.I(GBA_A_out),
		.T(!GBA_A_oe)
	);
	
	Top top (
		.clock(sys_clk),
		.reset(sys_rst),

		.io_gbaHost_CLK(GBA_CLK),
		.io_gbaHost_nWR(GBA_nWR),
		.io_gbaHost_nRD(GBA_nRD),
		.io_gbaHost_nCS(GBA_nCS),
		.io_gbaHost_CS2(GBA_CS2),
		.io_gbaHost_nREQ(GBA_nREQ),
		.io_gbaHost_VDD(GBA_VDD),

		.io_gbaHost_AD_in(GBA_AD_in),
		.io_gbaHost_AD_out(GBA_AD_out),
		.io_gbaHost_AD_oe(GBA_AD_oe),
		.io_gbaHost_A_in(GBA_A_in),
		.io_gbaHost_A_out(GBA_A_out),
		.io_gbaHost_A_oe(GBA_A_oe),

		.io_blinkyHost_led(led)
	);

endmodule

