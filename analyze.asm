  # analyze.asm
  # This file written 2015 by F Lundevall
  # Copyright abandoned - this file is in the public domain.

	.text
main:
	li	$s0,0x30 # 0x30 = 3*16 = 48 /LB
loop:
	move	$a0,$s0		# copy from s0 to a0
	
	li	$v0,11		# syscall with v0 = 11 will print out
	syscall			# one byte from a0 to the Run I/O window

	addi	$s0,$s0,3	# what happens if the constant is changed?
	
	li	$t0,0x5d        # 0x5b = 5*16+11 = 91 /LB ändrade till 0x5a för att stanna på Z
	bne	$s0,$t0,loop    # bne -> s0 != t0 then -> loop /LB
	nop			# delay slot filler (just in case)

stop:	j	stop		# loop forever here
	nop			# delay slot filler (just in case)

