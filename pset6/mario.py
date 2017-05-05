import cs50

while True:
    print("Height:",end="")
    s=cs50.get_int()
    if s >0 and s<24:
        break

for x in range(s):
    print(" "*(s-x),end="")
    print("#"*(x+1),end="")
    print("  ",end="")
    print("#"*(x+1))