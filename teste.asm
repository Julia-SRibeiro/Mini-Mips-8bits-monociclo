    addi $t2, $t0, 59 
    addi $t1, $t0, 3 
    add $t3, $t2, $t1 
    add $t3, $t2, $t1 
    addi $t4, $t0, 10 
    sw $t2, 0($t0) 
    lw $t1, 4($t0) 
    beq $t0, $t2, 8 
    beq $t0, $t5, 1 
    beq $t3, $t2, 10 
    j 1 
