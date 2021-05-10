`timescale 1ns / 1ns
`default_nettype none

module top_tb;
	reg clk = 0;
	always #5 begin
		clk = ~clk;
	end

	reg rst;
	reg GBA_nWR;
	reg GBA_nRD;
	reg GBA_nCS;
	reg GBA_nCS2;
	reg  [15:0] GBA_AD_in;
	wire [15:0] GBA_AD_out;
	wire GBA_AD_oe;
	reg  [7:0] GBA_A_in;
	wire [7:0] GBA_A_out;
	wire GBA_A_oe;

	initial begin
		$display("Starting simulation");
		$dumpfile("dump.vcd");
		$dumpvars(0, top_tb);

		GBA_nWR <= 1'b1;
		GBA_nRD <= 1'b1;
		GBA_nCS <= 1'b1;
		GBA_nCS2 <= 1'b1;
		GBA_AD_in <= 16'hFF;
		GBA_A_in <= 16'hFF;

		rst <= 1'b0;
		#5;
		rst <= 1'b1;
		#40;

		GBA_nCS2 <= 1'b0;
		#20;
		GBA_AD_in <= 16'h00;
		GBA_A_in <= 16'haa;
		#20;
		GBA_nWR <= 1'b0;
		GBA_nRD <= 1'b1;
		#100;
		GBA_nWR <= 1'b1;
		#500;

		GBA_AD_in <= 16'h01;
		GBA_A_in <= 16'h55;
		#20;
		GBA_nWR <= 1'b0;
		#100;
		GBA_nWR <= 1'b1;
		#20;
		GBA_nCS2 <= 1'b1;
		#500

		GBA_nCS2 <= 1'b0;
		#20;
		GBA_AD_in <= 16'h00;
		#20;
		GBA_nWR <= 1'b1;
		GBA_nRD <= 1'b0;
		#100;
		GBA_nRD <= 1'b1;
		#500;

		GBA_AD_in <= 16'h01;
		#20;
		GBA_nRD <= 1'b0;
		#100;
		GBA_nRD <= 1'b1;
		#20;
		GBA_nCS2 <= 1'b1;
		#500

		$finish;
	end

	Top top (
		.clock(clk),
		.reset(rst),

		.io_host_gba_CLK(1'b0),
		//.io_host_gba_nWR(btn[0]),
		//.io_host_gba_nRD(btn[1]),
		//.io_host_gba_nCS(btn[2]),
		//.io_host_gba_nCS2(btn[3]),
		.io_host_gba_nWR(GBA_nWR),
		.io_host_gba_nRD(GBA_nRD),
		.io_host_gba_nCS(GBA_nCS),
		.io_host_gba_nCS2(GBA_nCS2),
		.io_host_gba_nREQ(),
		.io_host_gba_VDD(1'b1),

		.io_host_gba_AD_in(GBA_AD_in),
		.io_host_gba_AD_out(GBA_AD_out),
		.io_host_gba_AD_oe(GBA_AD_oe),
		.io_host_gba_A_in(GBA_A_in),
		.io_host_gba_A_out(GBA_A_out),
		.io_host_gba_A_oe(GBA_A_oe),

		.io_board_gba_debug(),
		.io_board_blinky_led()
	);
endmodule

`default_nettype wire
