.set noat

.global _start
.ent    _start 
_start:
  
	addiu $t0, $0, 0xfffd  # treated as unsigned, 
	sltiu $t1, $t0, 1 #$t1 should be 0
 	li $t3, 0
	bne $t1, $t3, fail
	
	sltiu $t1, $t0, 0xfffe  #t1 should be 1 because t0 < imm
	beq $t1, $t3, fail
	
	li $t0, 0xffff5555
	li $t1, 0x0
	andi $t2, $t1, 0xaaaa
	bne $t1, $t2, fail
	j end
	
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
str: .asciiz "something bad!\n"
