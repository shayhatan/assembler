mcr r1
mov reg1, val
add reg2, reg1
endmcr
r1

mcr hlt
mov reg1, val
add reg2, reg1
endmcr
hlt

mcr .data
mov reg1, val
add reg2, reg1
endmcr
BLA: .data 12