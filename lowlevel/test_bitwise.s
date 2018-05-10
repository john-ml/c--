; assemble with `python asm.py --word-size=8 --mem-size=100`
    
; 5 = 0b0101
loadimm  5
  store  99

; 12 = 0b1100
loadimm  12
    and  99

; expect mem[99] = 0b100
  store  99

; repeat process with or and mem[98]
; expect mem[98] = 0b1101
loadimm  5
  store  98
loadimm  12
     or  98
  store  98

; check not; expect mem[97] = 1...10
loadimm  1
  store  97

; the argument doesn't really matter
    not  0
  store  97

; repeat infinitely
    jmp  0

