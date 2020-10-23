.text
.set noat

.global _start
.ent    _start 
_start:
  

  addiu $t1, $0, -4
  addiu $t2, $0, 2
  mult $t1, $t2
  li $t3, -8
  mflo $t4
  bne $t3, $t4, fail
  
  addiu $t1, $0, -4 # -4 in 16 bits:
  # 	1111 1111 1111 1100 treated as unsiged its 2^(16) - 4 = 65,532 = 0xFFFC
  addiu $t2, $0, 4   #65532 * 4 = 0x3FFF0
  multu $t1, $t2
  li $t3, 0x0003 # = upper 32 bits of product
  addiu $t4, $0, 0xFFF0 #lower 32 bits of product
  mfhi $t5
  mflo $t6
  bne $t3, $t5, fail
  bne $t4, $t6, fail	
  
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
