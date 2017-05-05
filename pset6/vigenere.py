import cs50
import sys

if (len(sys.argv)) != 2:
    print("Usage vigenere.py k")
    exit(1)
if not sys.argv[1].isalpha():
    print("Key must be alphabetical")
    exit(1)
    
cipher = []
leng = len(sys.argv[1])
for i in range(leng):
    if sys.argv[1][i].isupper():
        cipher=cipher + [ord(sys.argv[1][i])-65]
    if sys.argv[1][i].islower():
        cipher=cipher + [ord(sys.argv[1][i])-97]

print("Plaintext: ",end="")
plain = cs50.get_string()
print("Ciphertext: ",end="")
j=0
for c in plain:
    if not c.isalpha():
        print(c,end="")
    if c.isupper():
        print("{}".format(chr((ord(c)-65+cipher[j%leng])%26+65)),end="")
        j+=1
    if c.islower():
        print("{}".format(chr((ord(c)-97+cipher[j%leng])%26+97)),end="")
        j+=1
print("")    
    