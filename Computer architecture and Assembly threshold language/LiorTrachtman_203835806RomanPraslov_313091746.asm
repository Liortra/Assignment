# Title: EXE 6	name: Lior Trachtman && Roman Praslov
################# Data segment #####################
.data
jumptable: .word top,add_number,replace,del,find,average,max,print_array,sort,end
array1:  .word 0:30  # array of 30 words
array2:  .word 0:30 #  array temp of 30 words for sort 
Num:  .word 0
msg1: .asciiz "\nThe options are:\n1. Enter a number (base 10)\n2. Replace a number (base 10)\n3. DEL a number (base 10)\n4. Find a number in the array (base 10)\n5. Find average (base 2-10)\n6. Find Max (base 2-10)\n7. Print the array elements (base 2-10)\n8. Print sort array (base 2-10)\n9. END\n"
msg2: .asciiz "What number to add?\n"
msg3: .asciiz "What number to replace?\n"
msg4: .asciiz "What number to delete?\n"
msg5: .asciiz "What number to find?\n"
ok: .asciiz "\nIt is ok"
fullM: .asciiz "The array is full.\n"
existM: .asciiz "The number exist in index:"
NexistM: .asciiz "The number isn't exist"
emptyM: .asciiz "The array is empty.\n"
numM: .asciiz "The number that was found: "
indexM: .asciiz "\nin index: "
rep2: .asciiz "Replace the number "
rep3: .asciiz "(in index "
rep4: .asciiz ")with what number? "
printBase: .asciiz "Which base would you like to print the number? "
ResultM: .asciiz "\nResult: "
################# Code segment #####################
.text
.globl main
main:	# main program entry
	la $a1,Num
	la $a2,array1
	la $a3,array2
top:		
	li $v0,4		# Code to print a string
	la $a0,msg1
	syscall
	li $v0,5		# Code to read an integer
	syscall
	blez $v0,top		# Default for less than one
	li $t3,9
	bgt $v0,$t3,top		# Default for greater than 9
	move $s0,$v0		# Get index in $s0
	sll $s0,$s0,2		# Compute word offset/index = i*4
	la $a0,jumptable	# Load address of jumptable
	add $t0,$a0,$s0		# Form a pointer into jumptable
	lw $t1,($t0)		# Load an address from jumptable  
	jalr $t1
	j top			# Jump to specific case “switch”
end:
 	li $v0, 10		# Exit program
	syscall	
####################################################
#Procedure:  Adding a number                       #	
####################################################
add_number:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,0($a1)
	beq $t0,30,full		# Check NUM first
	li $v0,4		# Code to print a string
	la $a0,msg2
	syscall
	li $v0,5		# Code to read an integer
	syscall
	move $a0,$v0		
	jal check
	bne $v0,-1,yes
	sw $a0,0($a2)
	addi $a2,$a2,4
	addi $t0,$t0,1		# NUM++
	sw $t0,0($a1)		# Store the new NUM in its place
	li $v0,4		# Code to print a string
	la $a0,ok
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
yes:	
	sll $t8,$t0,2
	sub $t9,$a2,$t8
	sub $t0,$v0,$t9
	srl $t0,$t0,2
	li $v0,4		# Code to print a string
	la $a0,existM
	syscall
	li  $v0, 1    
    	add $a0, $t0, $zero 	# Print the index
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
full:
	li $v0,4		# Code to print a string
	la $a0,fullM
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  Replace a number                      #	
####################################################
replace:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	beqz $t0,empty		# Xheck NUM first
	li $v0,4
	la $a0,msg3		# Code to print a string
	syscall			
	li $v0,5		# Code to read an integer
	syscall
	move $a0,$v0		# The num that I want to change
	jal check
	beq $v0,-1,no
	move $t1,$v0		# The number that I want to print
	sll $t8,$t0,2
	sub $t9,$a2,$t8
	sub $t5,$v0,$t9
	srl $t5,$t5,2		# The index of the number
	addi $t5,$t5,1		# Add 1 to start counting from 1
	li $v0,4		# Code to print a string
	la $a0,rep2
	syscall
	li $v0,1		# Print the number
	lw $a0,($t1)
	syscall
	li $v0,4		# Code to print a string
	la $a0,rep3
	syscall
	li $v0,1		# Print the index
	add $a0,$t5,$zero	
	syscall
	li $v0,4		# Code to print a string
	la $a0,rep4
	syscall
	li $v0,5		# The new number
	syscall
	sw $v0,0($t1)
	li $v0,4		# Code to print a string
	la $a0,ok
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
no:
	move $t1,$a0
	li $v0,4		# Code to print a string
	la $a0,NexistM
	syscall
	li  $v0,1    
    	add $a0,$t1, $zero 
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra	
empty:
	li $v0,4		# Code to print a string
	la $a0,empty
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  delete a number                       #	
####################################################
del:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	beqz $t0,empty		# Check NUM first
	li $v0,4
	la $a0,msg4		# Code to print a string
	syscall			
	li $v0,5		# Code to read an integer
	syscall
	move $a0,$v0		# The num that I want to delete
	jal check
	beq $v0,-1,no
	sw $zero,0($v0)		# Put zero in the place that i want to delete
reduction:			# Now need to move down all the other numbers
	lw $t3,4($v0)
	sw $t3,($v0)
	addi $v0,$v0,4
	bne $v0,$a2,reduction
	addi $a2,$a2,-4
	addi $t0,$t0,-1		# NUM--
	sw $t0,($a1)		# Store the new NUM
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra		
####################################################
#Procedure:  Find a number                         #	
####################################################
find:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	li $v0,4
	la $a0,msg5		# Code to print a string
	syscall
	li $v0,5		# Code to read an integer
	syscall
	move $a0,$v0		# The num that I want to find
	jal check
	beq $v0,-1,no
	lw $t4,0($v0)		# The number that I find
	sll $t8,$t0,2
	sub $t9,$a2,$t8
	sub $t5,$v0,$t9
	srl $t5,$t5,2		# The index of the number
	addi $t5,$t5,1		# Add 1 to start counting from 1
	li $v0,4
	la $a0,numM		# Code to print a that we find
	syscall
	li  $v0, 1    
    	add $a0,$t4,$zero 	# Print the num that we find
	syscall
	li $v0,4
	la $a0,indexM		# Code to print the index that we find
	syscall
	li  $v0, 1    
    	add $a0,$t5,$zero 	# Print the index of the num that we find
	syscall
	li $v0,4		# Code to print a string
	la $a0,ok
	syscall
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  Replace a number                      #	
####################################################
average:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	sll $t8,$t0,2
	sub $t9,$a2,$t8
	li $t8,0
sum:
	lw $t7,0($t9)
	add $t8,$t8,$t7
	addi $t9,$t9,4
	blt $t9,$a2,sum
	div $t7,$t8,$t0
	jal Qprint
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  The max                               #	
####################################################
max:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	sll $t8,$t0,2
	sub $t9,$a2,$t8
	li $t8,0
	lw $t7,0($t9)
	addi $t9,$t9,4
wmax:
	lw $t6,0($t9)
	bgt $t6,$t7,change
	addi $t9,$t9,4
	blt $t9,$a2,wmax
	jal Qprint
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
change:
	move $t7,$t6
	addi $t9,$t9,4
	blt $t9,$a2,wmax
	jal Qprint
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  Print the array                       #	
####################################################
print_array:
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	lw $t0,($a1)
	li $v0,4		# Code to print a string
	la $a0,printBase
	syscall
	li $v0,5		# Code to read an integer
	syscall	
	move $t6,$v0		# The base that I want to print
	sll $t8,$t0,2
	sub $t9,$a2,$t8
loopP:	
	lw $t7,0($t9)
	jal basePrint
	addi $t9,$t9,4
	blt $t9,$a2,loopP
	lw $ra,0($sp)
	addiu $sp, $sp,4
	jr $ra
####################################################
#Procedure:  Sort the array                        #	
####################################################
sort:
   
####################################################
#Procedure:  check                      	   #	
####################################################
check:
	sll $t8,$t0,2
	sub $t9,$a2,$t8
loop:
	lw $t7,0($t9)
	beq $t7,$a0,found
	addi $t9,$t9,4
	blt $t9,$a2,loop
	li $v0,-1
	jr $ra
found:
	move $v0,$t9
	jr $ra	
####################################################	
Qprint:	
	li $v0,4		# Code to print a string
	la $a0,printBase
	syscall
	li $v0,5		# Code to read an integer
	syscall
	move $t6,$v0		# The base that I want to print
basePrint:	
	addiu $sp,$sp,-4
	sw $ra,0($sp)
	addiu $sp,$sp,-4
	sw $a1,0($sp)		# Save the old a1-NUM in stack
	addiu $sp,$sp,-4
	sw $a2,0($sp)		# Save the old  a2-array1 in stack
	move $a1,$t7		# The number that I want to print
	move $a2,$t6		# a1-the number that I want to print a2-the base	
	jal print_num
backto:
	lw $a2, 0($sp)		# Get back the array in a2
	addiu $sp, $sp,4
	lw $a1, 0($sp)		# Get back the NUM in a1
	addiu $sp, $sp,4
	lw $ra, 0($sp)		# Get back place that call me
	addiu $sp, $sp,4
	jr $ra   		# Return
####################################################
#Procedure:  print number                    	   #	
####################################################
print_num:  			# Print number by a chosen base
	li $v0,4
	la $a0,ResultM
	syscall
	li $s3,0 		# Counter,used to know how many times we will pop from stack
	bge $a1,0,convert
NorP:				# Get the absolute value of the number to print
	li $v0, 11		# Print char
	li $a0, '-'		# Print '-'
	syscall
	abs $a1, $a1	
convert:
	#a1=A #a2=B A-number B-base
	beqz $a1,finish
	div $t4,$a1,$a2 	# t4=A/B
	rem $t3,$a1,$a2	 	# t3=A%B
	add $sp,$sp,-4
	sw $t3,0($sp) 	 	# Save t3
	add $a1,$zero,$t4 	# Pass A/B
	addi $s3,$s3,1
	j convert       	# Call convert
finish:
	beqz $s3,done
	lw $a0,0($sp)
	addiu $sp, $sp,4
	li $v0,1
	syscall
	addi $s3,$s3,-1		
	j finish
done:	
	jr $ra
