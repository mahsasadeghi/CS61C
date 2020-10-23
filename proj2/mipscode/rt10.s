.text
.set noat

.global _start
.ent    _start 
_start:
  
  la $t0, x
  addiu $t1, $0, 0xff
  sb $t1, 0($t0)
  lbu $t2, 0($t0)
  addiu $t3, $0, 4
  sb $t3, 1($t0)
  lbu $t4, 1($t0)

  addiu $t0, $t0, 4

  lbu $t1, -4($t0)
  lbu $t1, -2($t0)

  li $v0, 10
  syscall

_end:

.end _start

.data
x: .word 0x01234567
