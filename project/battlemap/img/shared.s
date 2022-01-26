
@{{BLOCK(shared)

@=======================================================================
@
@	shared, 16x16@4, 
@	+ palette 25 entries, not compressed
@	Total size: 50 = 50
@
@	Time-stamp: 2021-12-27, 21:14:36
@	Exported by Cearn's GBA Image Transmogrifier, v0.8.17
@	( http://www.coranac.com/projects/#grit )
@
@=======================================================================

	.section .rodata
	.align	2
	.global sharedPal		@ 52 unsigned chars
	.hidden sharedPal
sharedPal:
	.hword 0x0000,0x7500,0x0115,0x7F6E,0x1CC0,0x023D,0x7DEA,0x03DF
	.hword 0x1DC0,0x6BF5,0x7BF8,0x0021,0x77FF,0x203A,0x7DEA,0x0115
	.hword 0x1CC0,0x1DC0,0x6BF4,0x143E,0x7BF8,0x0020,0x7F4D,0x209C
	.hword 0x77FF

@}}BLOCK(shared)
