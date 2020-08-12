import matplotlib.pyplot as plt

size = 2000

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

plt.figure(1)
plt.scatter(y, x, marker='.', linewidths=0.01)
plt.plot([0, size-1], [size-1, 0], color='red')


plt.figure(2)


file = open("output/output_matrix.txt", "r")
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

plt.scatter(x, y, marker=".",linewidths=0.01)
plt.plot([0, size-1], [size-1, 0], color='red')
plt.show()
