.text
.set noat

.global _start
.ent    _start 
_start:
  
  la $t0, x
  addiu $t1, $0, 0xfcde
  sw $t1, 0($t0)
  lw $t2, 0($t0)
  addiu $t1, $0, 0x2432
  sw $t1, 4($t0)
  lw $t4, 4($t0)

  

  li $v0, 10
  syscall

_end:

.end _start

.data
x: .word 0x01234567
