; store TMP into address 9, then hang
; assemble with `python asm.py --word-size=4 --mem-size=10`
    
loadimm 15
store   9
jmp     0

