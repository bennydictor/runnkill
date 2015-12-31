import colored

fil = open("field")
a = fil.read().split()
i = 0
for l in a:
    for c in l:
        print(colored.Print(' ', bg=("gray" if c == '0' else ("red" if c == '1' else "black"))), end='')
    print(i)
    i += 1
for i in range(len(a[0])):
    print(i, end = '')

print(colored.Print('1', "black", "red"))

