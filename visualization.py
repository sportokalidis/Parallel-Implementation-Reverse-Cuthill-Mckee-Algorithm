import matplotlib.pyplot as plt

size = 10

file = open("input/test_input.txt", "r")
x = []
y = []
Xcounter=size-1
Ycounter = 0
file = list(file.read())

for i in file:
    if(i == '0'):
        Ycounter += 1
        if(Ycounter == size):
            Xcounter -= 1
            Ycounter = 0
    if(i == '1'):
        x.append(Xcounter)
        y.append(Ycounter)
        Ycounter += 1
        if(Ycounter == size):
            Xcounter -= 1
            Ycounter = 0
    if(i == '\n'):
        continue

plt.scatter(y, x)
plt.show()



file = open("output/output.txt", "r")
x = []
y = []
Xcounter=size-1
Ycounter = 0
file = list(file.read())

for i in file:
    if(i == '0'):
        Ycounter += 1
        if(Ycounter == size):
            Xcounter -= 1
            Ycounter = 0
    if(i == '1'):
        x.append(Xcounter)
        y.append(Ycounter)
        Ycounter += 1
        if(Ycounter == size):
            Xcounter -= 1
            Ycounter = 0
    if(i == '\n'):
        continue

plt.scatter(x, y)
plt.show()
