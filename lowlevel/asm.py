import sys

argv = []
outfile = None
word_size = None
mem_size = None

def binarize(i, word_size=word_size):
    return "{0:b}".format(i).zfill(word_size)

def isflag(s, abbreviated, full):
    return s.startswith("-%s=" % abbreviated) or s.startswith("--%s=" % full)

def input_generator():
    try:
        while True:
            yield input()
    except EOFError:
        return

for arg in sys.argv:
    if isflag(arg, "o", "output"):
        outfile = arg.split("=")[1]
    elif isflag(arg, "w", "word-size"):
        word_size = int(arg.split("=")[1])
    elif isflag(arg, "m", "mem-size"):
        mem_size = int(arg.split("=")[1])
    else:
        argv.append(arg)

argv.pop(0) # remove script name

if word_size is None:
    print("Error: word size not specified.")
    sys.exit(1)

if mem_size is None:
    print("Error: memory size not specified.")
    sys.exit(1)

if len(argv) > 1:
    print("Error: too many input files.")
    sys.exit(1)

if len(argv) < 1:
    infile = input_generator()
    close_infile = lambda: None
else:
    infile = open(argv[0], "r")
    close_infile = infile.close

if outfile is None:
    write = sys.stdout.write
    close_writer = lambda: None
else:
    output = open(outfile, "w")
    write = output.write
    close_writer = output.close

def done(code=0):
    close_infile()
    close_writer()
    sys.exit(code)

for i in range(word_size):
    b = binarize(i, word_size)
    write("#define IP_%s %s\n" % (b, b))
    write("#define AP_%s %s\n" % (b, binarize(word_size + i, word_size)))
    write("#define TMP_%s 0\n" % b)

mem_ptr = 0
for line in infile:
    if line.strip() == "": # empty lines
        continue
    if line[0] == ';': # basic comments
        continue

    # split() splits on sequences of whitespace
    tokens = line.split()
    if len(tokens) != 2:
        print("Error: invalid instruction: %s" % line)
        done(1)
    op, arg = tokens[0], binarize(int(tokens[1]), word_size)
    if len(arg) > word_size:
        print("Error: the value %d in the instruction '%s' does not fit in 1 word (word-size=%d)" % (
            int(tokens[1]), line, word_size
        ))
        done(1)
    for i in range(word_size): # write instruction
        if mem_ptr >= word_size * mem_size:
            print("Error: not enough memory for executable.")
            done(1)
        write("#define MEM_%s %s_%d\n" % (
            binarize(mem_ptr, word_size),
            op.upper(),
            i
        ))
        mem_ptr += 1
    for i in range(word_size):
        if mem_ptr >= word_size * mem_size:
            print("Error: not enough memory for executable.")
            done(1)
        write("#define MEM_%s %s\n" % (
            binarize(mem_ptr, word_size),
            arg[i]
        ))
        mem_ptr += 1

# pad the rest of memory with zeros
for i in range(mem_ptr, word_size * mem_size):
    write("#define MEM_%s 0\n" % binarize(i, word_size))
        
done()
