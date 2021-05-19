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

		#10000

		// nCS = 0
		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h00;    // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		// Send DUMMY
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'hFF;    // 0xFF
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send CMD0
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h40;    // 0x40
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send ARG[3]
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h00;    // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send ARG[2]
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h00;    // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send ARG[1]
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h00;    // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send ARG[0]
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h00;    // 0x00
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send CRC
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'h95;    // 0x95
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Send DUMMY
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'hFF;    // 0xFF
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// Read response?
		addr <= 16'h0000; // SPI_DATA
		mosi <= 8'hFF;    // 0xFF
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h01;    // 0x01
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

		#50000

		// nCS = 1
		addr <= 16'h0001; // SPI_CTRL
		mosi <= 8'h02;    // 0x02
		write <= 1'b1;
		#30
		write <= 1'b0;
		#20

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
