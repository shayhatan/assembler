.define len = 4
STR: .string "abcdef"
LIST: .data 6, -9, len
K: .data 22

mcr M2
mov reg1, val
add reg2, reg1
endmaacr

M2