MOD: 
	lui $r2, 0
	sw $r3, 3($r2)  #store $r3 so we can use it
	lw $r1, 1($r2)
	lw $r0, 0($r2)
	lui $r2, 0x80
	and $r3, $r1, $r2 #check the sign of $r1
	lui $r2, 0x0
	beq $r3, $r2, next #if $r3 is positive
	sub $r1,$r2,$r1 #  make $r1 positive by sub from 0

next: lui $r3, 0 #use $r3 as zero register 
      slt $r2,$r0,$r1 # check if $r0 < $r1
      beq $r2,$r3,loop
      j end
loop:
    sub $r0, $r0, $r1 #subtract $r0-$r1
    slt $r2,$r0,$r1
    beq $r3, $r2, loop
end:
    sw  $r0, 2($r3)
	lui $r1, 0
	lw $r3,3($r1) # restore $r3
	jr $r3