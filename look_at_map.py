import colored

fil = open("field")
a = fil.read().split()
for l in a:
    for c in l:
        print(colored.Print(' ', bg=("gray" if c == '0' else ("red" if c == '1' else "black"))), end='')
    print()
print(colored.Print('1', "black", "red"))
