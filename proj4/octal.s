OCTAL:
	lui $r0, 0
	lui $r1, 0
	lui $r2, 0
	sw $r3, 1($r0)

	lw $r1 0($r0)   
	addi $r2 $r2 7  
	sllv $r1 $r1 $r2
	srlv $r1 $r1 $r2 

	lui $r3, 0	
	add $r3 $r3 $r1 

	lui  $r2, 0
	addi $r2, $r2, 6
	srlv $r3 $r3 $r2  

	lui  $r2, 0
	addi $r2, $r2, 8
	sllv $r0 $r3 $r2  

	lui  $r3, 0
	add $r3 $r3 $r1  

	lui  $r2, 0
	addi $r2, $r2, 10
	sllv $r3 $r3 $r2 

	lui  $r2, 0
	addi $r2, $r2, 13
	srlv $r3 $r3 $r2 

	lui  $r2, 0
	addi $r2, $r2, 4
	sllv $r3 $r3 $r2 
	or $r0 $r0 $r3 

	lui $r3, 0
	add $r3 $r3 $r1 

	lui  $r2, 0
	addi $r2, $r2, 13
	sllv $r3 $r3 $r2 
	srlv $r3 $r3 $r2 
	or $r0 $r0 $r3 

	disp $r0 0  

	lui $r0, 0
	lw $r3, 1($r0)
	jr $r3
