`timescale 1ns / 1ps
`default_nettype none

module FPGATop (
	input wire sys_clk,
	input wire sys_rst,

	input wire uart_rx,
	output wire uart_tx,
	output wire [7:0] debug,

	input wire [3:0] btn,
	output wire [3:0] led,

	output wire spi_cs,
	output wire spi_sck,
	output wire spi_mosi,
	input wire spi_miso,

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

		.io_host_gba_CLK(GBA_CLK),
		.io_host_gba_nWR(GBA_nWR),
		.io_host_gba_nRD(GBA_nRD),
		.io_host_gba_nCS(GBA_nCS),
		.io_host_gba_nCS2(GBA_nCS2),
		.io_host_gba_nREQ(GBA_nREQ),
		.io_host_gba_VDD(GBA_VDD),

		.io_host_gba_AD_in(GBA_AD_in),
		.io_host_gba_AD_out(GBA_AD_out),
		.io_host_gba_AD_oe(GBA_AD_oe),
		.io_host_gba_A_in(GBA_A_in),
		.io_host_gba_A_out(GBA_A_out),
		.io_host_gba_A_oe(GBA_A_oe),

		.io_board_sd_spi_cs(spi_cs),
		.io_board_sd_spi_sck(spi_sck),
		.io_board_sd_spi_mosi(spi_mosi),
		.io_board_sd_spi_miso(spi_miso),

		.io_board_gba_debug(),
		.io_board_sd_debug(debug),
		.io_board_blinky_led(led)
	);

endmodule

`default_nettype wire
