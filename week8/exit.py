import sys

if len(sys.argv) != 2:
    print("incorrect # of command line args")
    exit(1)
    
print("hello {}".format(sys.argv[1]))
exit(0)