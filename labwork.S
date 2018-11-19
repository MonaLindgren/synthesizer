# labwork.S
# Written 2015-2017 by F Lundevall
# Skeleton file for IS1200/IS1500 lab 1.
# The macros PUSH and POP are in the public domain.
# Please add your own code at the end of the file.

#
# Please keep the two macros PUSH and POP unchanged
#
.macro	PUSH reg
addi	$sp,$sp,-4
sw	\reg,0($sp)
.endm

.macro	POP reg
lw	\reg,0($sp)
addi	$sp,$sp,4
.endm
#
# Please add your own code below this line
#

.global enable_interrupt
enable_interrupt:
ei
jr $ra
nop

.global hexasc
hexasc:
andi $a0, $a0, 0xf	# nollställ alla andra bitar utom 4 l.s. bits
addi $v0, $a0, 0x30	# lägger till 48 för att komma upp i motsvarande asciikod för 0-9
li $t0, 0x39		# laddar in gränsen för asciikoden till 9.
ble $v0, $t0, return	# hoppa direkt till return om input är lägre än villkoret (talet 9)
nop
addi $v0, $v0, 0x7	# annars, lägg till 7 för att hoppa över glappet av asciitecken :, ; <  = osv.
return:	andi $v0, $v0, 0x7f	# nollställ alla andra bitar utom 7 st least significant.
jr $ra
nop			# returnera.
.global delay
delay:
move $t0, $a0

whileloop:
beq $t0,0, stop
nop
sub $t0,$t0,1
li $t1,0
forloop:
beq $t1, 4000, whileloop
nop
add $t1, $t1, 1
j forloop
nop

stop:
jr $ra
nop

.global	time2string
time2string:
PUSH $ra ##Pushar returadress + s-register
PUSH $s0
PUSH $s1
move $s0, $a0 #spara returadressen temporärt
move $s1, $a1
move $a0, $a1 #hexasc har in-argument a0 därmed flytta över a1 till a0
jal hexasc
move $v1, $v0
nop	# en asciikod finns i v0, laddas in i V0 från hexasc hexasc använder endast 4-LSB
sb $v0, 4($s0)  #sparar v0 på minnesplatsen $s0
srl $a0, $s1, 4  #shiftar för att få ut den andra 4-bitssiffran
jal hexasc
nop
sb $v0, 3($s0)  #sparar ny v0 på minnesplatsen $s0
srl $a0, $s1, 8
jal hexasc
nop
sb $v0, 1($s0)  #sparar ny v0 på minnesplatsen $s0
srl $a0, $s1, 12
jal hexasc
nop
sb $v0, 0($s0)  #sparar ny v0 på minnesplatsen $s0
li $v0, 0x3a  #laddar in asciikoden för ":" i $v0
sb $v0, 2($s0)  #spara ":" v0 mellan sparade siffror
li $v0, 0x00  #ASCII NUL
sb $v0, 5($s0)  #sparar null byte
POP $s1
POP $s0
POP $ra
jr $ra
nop
