main:
    addi $r2, $r0, -5 
    addi $r1, $r0, 3 
    add $r3, $r2, $r1 
    add $r3, $r2, $r1 
    addi $r4, $r0, 10 
    sw $r2, 0($0) 
    lw $r1, 4($0) 
    beq $r2, $r0, 8 
    beq $r5, $r0, 1 
    beq $r2, $r3, 10 
    j 1 
