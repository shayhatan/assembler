//
// Created by User on 28/02/2024.
//

#ifndef ASSEMBLER_CONSTS_H
#define ASSEMBLER_CONSTS_H


/* parsed opcodes */
#define absolute = "00"
#define relative = "01"
#define external = "10"

/* instructions */
enum opcode {
    mov = 0,
    cmp = 1,
    add = 2,
    sub = 3,
    not = 4,
    clr = 5,
    lea = 6,
    inc = 7,
    dec = 8,
    jmp = 9,
    bne = 10,
    red = 11,
    prn = 12,
    jsr = 13,
    rts = 14,
    hlt = 15
};

#endif //ASSEMBLER_CONSTS_H
