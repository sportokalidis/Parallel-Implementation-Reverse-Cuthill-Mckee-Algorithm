from scipy.sparse import csr_matrix
from scipy.sparse.csgraph import reverse_cuthill_mckee


file = open("input/test_input.txt", "r")
graph = [[]]
counter=0;

file = list(file.read())
# print(file)
for i in file:
    if(i == '0' or i == '1'):
        value = int(i)
        graph[counter].append(value)
    if(i =='\n'):
        counter += 1
        graph.append([])


graph.remove([])
# print(graph)
print(len(graph))

graph = csr_matrix(graph)

# print(graph)

result = reverse_cuthill_mckee(graph)

print(result)
