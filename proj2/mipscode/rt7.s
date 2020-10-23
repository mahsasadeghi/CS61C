.text
.set noat

.global _start
.ent    _start 
_start:
  
  la $t0, x
  addiu $t1, $0, 0x1111
  addiu $t2, $0,  0x0001
  sll $t3, $t1, 15
  sll $t4, $t2, 30
  bne $t3, $t3, fail
  
  addiu $t1, $0, 0x8000
  addiu $t2, $0, 0x0001
  srl $t1, $t1, 31
  bne $t1, $t2, fail
  
  
  addiu $t1, $0, 0xf000
  addiu $t2, $0, 0xffff
  sra $t1, $t1, 12
  beq $t1, $t2, end
  j fail
  
  
fail:
  	ori   $a0, %lo(str)
  	ori   $v0, $zero, 4
	syscall
	j end
  
end:
  li $v0, 10
  syscall

_end:

.end _start

.data
x: .word 0x01234567
str: .asciiz "something bad!\n"
