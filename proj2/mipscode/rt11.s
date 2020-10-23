.text
.set noat

.global _start
.ent    _start 
_start:
  
  la $t0, x
  li $t1, 0xfafa
  sh $t1, 0($t0)
  lhu $t2, 0($t0)
  addiu $t1, $0, 0x1bcd
  sh $t1, 2($t0)
  lhu $t4, 2($t0)

  

  li $v0, 10
  syscall

_end:

.end _start

.data
x: .word 0x01234567
