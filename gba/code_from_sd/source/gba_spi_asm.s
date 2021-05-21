.cpu arm7tdmi

.text
.global msec

.global spi_set_cs
.syntax unified
.arch armv4t
.code 16
.thumb_func
.fpu softvfp
.type spi_set_cs, %function

spi_set_cs:
	ldr r1, =0xE00F001 // SPI_CTRL
	ands r0, r0
	beq L2
	movs r0, #2
L2:
	strb r0, [r1]
	bx lr

.global spi_transfer
.syntax unified
.arch armv4t
.code 16
.thumb_func
.fpu softvfp
.type spi_transfer, %function

spi_transfer:
	// SPI_DATA = r0
	ldr r1, =0xE00F000 // SPI_DATA
	strb r0, [r1]

	// SPI_CTRL |= 0x01
	ldr r2, =0xE00F001 // SPI_CTRL
	ldrb r3, [r2]
	movs r0, #1
	orrs r3, r0
	strb r3, [r2]

	// Set timeout
	// r4 is address of msec
	// r5 is timeout value
	// r6 is used to read msec into
	push {r4, r5, r6}
	ldr r4, =msec
	ldr r6, [r4]
	movs r5, #100
	adds r5, r6

	// while(SPI_CTRL & 0x01);
L1:
	ldr r6, [r4] // read msec
	cmp r6, r5   // compare with timeout value
	blt L3       // continue to check status bit if msec < timeout

	// otherwise, timeout and return 0xFF
	pop {r4, r5, r6}
	movs r0, #0xFF
	bx lr
L3:
	ldrb r3, [r2]
	ands r3, r0
	bne L1

	pop {r4, r5, r6}

	// return SPI_DATA
	ldrb r0, [r1]
	bx lr
