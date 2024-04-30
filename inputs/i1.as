.define sz = 2
cmp  #qw		,,   #45
cmp  #qw		,   #45,
    cmp  #qw		,   #45,555
cmp  #qw		,   #45,555,

MAIN:
    mov r3, LIST[sz]
    mov 3, LIST[sz]

LOOP:
    jmp L1
    cmp r3, #sz
    bne END
prn #-5
mov STR[5], STR[2]
sub r1, r4

L1:
    inc K
    bne LOOP

END:
    hlt

DAM: jrr 15

TEST: mov r1
TEST1: mov r1, r2

TEST1: jmp r1, r2
TEST2: jmp r1, r2

.define r1=5
.define r1

.define len = fdsf
mov 4, r1

mov #4, r1

STR1:..string. "abcdef"
DAT:..data 14
DAT1:..data #14
DAT1:..data "aaa"

  bad comments
 unknown #13, r3
.define stttr = "-2"
.define stttr1 "-2"

.define len = 4
STR: .string "abcdef"
LIST: .data 6, -9, len
K: .data 22


    .entry .entry
    .entry .extern
LLL: .extern aaas
    .extern "BLA"
