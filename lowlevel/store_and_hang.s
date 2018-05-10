; store TMP into address 5, then hang
; assemble with `python asm.py --word-size=4 --mem-size=6`
    
store 5
jmp 0

