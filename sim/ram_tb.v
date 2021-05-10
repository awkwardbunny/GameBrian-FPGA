`timescale 1ns / 1ns
`default_nettype none

module ram_tb;
	reg clk = 0;
	always #5 begin
		clk = ~clk;
	end

	reg rst;
	reg [15:0] addr;
	reg [7:0] mosi;
	wire [7:0] miso;
	reg write;

	initial begin
		$display("Starting simulation");
		$dumpfile("dump.vcd");
		$dumpvars(0, ram_tb);

		rst <= 1'b1;
		addr <= 16'h0;
		mosi <= 8'h0;
		write <= 1'b0;
		#5
		rst <= 1'b0;
		#20;

		addr <= 16'h1;
		mosi <= 8'h42;
		write <= 1'b1;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		write <= 1'b0;
		#20

		addr <= 16'h2;
		mosi <= 8'hAA;
		write <= 1'b1;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		write <= 1'b0;
		#20

		addr <= 16'h3;
		mosi <= 8'h55;
		write <= 1'b1;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		write <= 1'b0;
		#20

		addr <= 16'h1;
		mosi <= 8'h0;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		#20

		addr <= 16'h3;
		mosi <= 8'h0;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		#20

		addr <= 16'h2;
		mosi <= 8'h0;
		#50
		addr <= 16'h0;
		mosi <= 8'h0;
		#20

		$finish;
	end

	GBARam gbaram (
		.clock(clk),
		.reset(rst),
		.io_card_bus_addr(addr),
		.io_card_bus_mosi(mosi),
		.io_card_bus_miso(miso),
		.io_card_bus_write(write)
	);
endmodule

`default_nettype wire
