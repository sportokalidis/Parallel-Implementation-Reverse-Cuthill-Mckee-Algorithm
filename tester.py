from termcolor import colored

filepy = open("output/py_output.txt", "r")
file0 = open("output/v0_output.txt", "r")
file1 = open("output/v1_output.txt", "r")
file2 = open("output/v2_output.txt", "r")

filepy = list(filepy)
file0 = list(file0)
file1 = list(file1)
file2 = list(file2)


print("---------------------------------------------------------")
if(filepy == file0):
    print("-> Version 0 is ", colored('Correct', 'green'))
else:
    print("-> Version 0 is ", colored('not Correct', 'red'))
if(filepy == file1):
    print("-> Version 1 is ", colored('Correct', 'green'))
else:
    print("-> Version 1 is ", colored('not Correct', 'red'))
if(filepy == file2):
    print("-> Version 2 is ", colored('Correct', 'green'))
else:
    print("-> Version 2 is ", colored('not Correct', 'red'))

print("---------------------------------------------------------")