import cs50

print("Number: ",end="")
num = cs50.get_float()

def checksum(num):
    working = num//10
    checksum = 0
    while working>0:
        if 2*(working%10) <10:
            checksum = checksum + 2*(working%10)
        else:
            checksum = checksum + 1 + (2*(working%10))-10
        working = working//100
    workinga = num
    while workinga>0:
        checksum = checksum + workinga%10
        workinga = workinga//100
    if checksum%10 == 0:
        return True
    else:
        return False

def cardtype(num):
    if(num//10000000000000 ==37 or num//10000000000000 ==34):
        print("AMEX")
    elif(num//100000000000000>50 and num//100000000000000<56):
        print("MASTERCARD")
    elif(num//1000000000000000==4 or num//1000000000000==4):
        print("VISA")
    else:
        print("INVALID")
    
if checksum(num):
    cardtype(num)
else:
    print("Invalid")