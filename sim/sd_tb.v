`timescale 1ns / 1ns
`default_nettype none

module sd_tb;
	reg clk = 0;
	always #5 begin
		clk = ~clk;
	end

	reg rst;
	reg [15:0] addr;
	reg [7:0] mosi;
	wire [7:0] miso;
	reg write;

	wire spi_cs;
	wire spi_sck;
	wire spi_mosi;
	reg  spi_miso;

	initial begin
		$display("Starting simulation");
		$dumpfile("dump.vcd");
		$dumpvars(0, sd_tb);

		// Reset values
		rst <= 1'b1;
		addr <= 16'h0;
		mosi <= 8'h0;
		write <= 1'b0;
		spi_miso <= 1'b1;
		#605
		rst <= 1'b0;
		#20;

		addr <= 16'h0; // REG_CMD
		mosi <= 8'h40; // CMD0
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h1; // REG_ARG0
		mosi <= 8'h00; // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h2; // REG_ARG1
		mosi <= 8'h00; // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h3; // REG_ARG2
		mosi <= 8'h00; // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h4; // REG_ARG3
		mosi <= 8'h00; // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h5; // REG_CRC
		mosi <= 8'h95; // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h9; // REG_STAT
		mosi <= 8'h01; // 0x80 // Trigger CMD
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#300000

		spi_miso <= 1'b0;
		#2500
		spi_miso <= 1'b1;
		#100000

		$finish;
	end

	SDCard sd (
		.clock(clk),
		.reset(rst),
		.io_card_bus_addr(addr),
		.io_card_bus_mosi(mosi),
		.io_card_bus_miso(miso),
		.io_card_bus_write(write),

		.io_board_spi_cs(spi_cs),
		.io_board_spi_sck(spi_sck),
		.io_board_spi_mosi(spi_mosi),
		.io_board_spi_miso(spi_miso)
	);
endmodule

`default_nettype wire
