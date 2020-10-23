	.data
# do not modify
LIST:	
	.word -44 -4 0 1 2 4 7 9 23 126
line:   
	.asciiz " was found at index "
newline: 
	.asciiz "\n"
expect:
	.asciiz ", expected "
	
	.text 
	
	li $s0 10 # length of the LIST
	li $s1 0
	
	la $a1 LIST # get address of the array
printLoop:
	li $a2 0 # search from index = 0
	li $a3 10 # to index = 10
	
	sll $t0 $s1 2
	addu $t0 $t0 $a1
	lw $s2 0($t0)
		
	move $a0 $s2
	li $v0 1
	syscall # PRINT value
	
	la $a0 line
	li $v0 4
	syscall # PRINT " was found at index "
	
	move $a0 $s2
	jal binSearch
	move $s2 $v0
	move $a0 $v0
	li $v0 1
	syscall # PRINT index
	
	beq $s2 $s1 skipError
	la $a0 expect
	li $v0 4
	syscall # PRINT ", expected "
	
	move $a0 $s1
	li $v0 1
	syscall # PRINT expected index
		
skipError:
	la $a0 newline
	li $v0 4
	syscall # PRINT "\n"
	
	addiu $s1 $s1 1 # s1++
	blt $s1 $s0 printLoop # repeat
	
	li $v0 10
	syscall # EXIT
	
# int binSearch(int value,int list[],int nmin,int nmax){	
binSearch: # binSearch($a0,$a1,$a2,$a3)
	# ****** YOUR CODE STARTS HERE ******
	#
	# Try not to deviate too greatly (if at all) from the C version.
        #
	# Ex: DO NOT look up the values of the $s registers, LIST, 
        # etc from above. You will receive no credit if you do.
        #
        # This problem can be solved using only the $a0-$a3,$t0-$t?,
        # $v0,$ra registers.
        
        # while(nmax>=nmin) {
        while:
		slt $t0 $a3 $a2
		bne $t0 $zero notFound
		
		# int nmid = (nmin+nmax)/2;
		add $t1 $a2 $a3
		srl $t1 $t1 1
		sll $t2 $t1 2
		
		# if(list[nmid]>value)
		add $t2 $a1 $t2
		lw $t3 0($t2)
		beq $a0 $t3 else	# list[nmid] = value
		slt $t4 $a0 $t3
		beq $t4 $zero elseIf	# list[nmid] < value

			#{nmax = nmid-1;}
			addi $a3 $t1 -1
			j while

		# else if(list[nmid]<value)
		elseIf:
			# {nmin = nmid+1;}
			addi $a2 $t1 1
			j while

		# else 
		else:
			# {return nmid;}
			add $v0 $t1 $zero
			j return	
	
notFound: # you can delete / alter this label and line as needed
	li $v0 -1 # return value = -1
	
return:
	jr $ra # return
