from math import lcm

nodes = {}

with open("2023/8/input.txt") as file:
    instructions = file.readline()[:-1]
    file.readline()  # '\n'
    for line in file:
        data = line.split()
        nodes[data[0]] = (data[2][1:-1], data[3][:-1])

steps = 0
current_nodes = [i for i in nodes if i[2] == "A"]
first_occurrence = [0] * len(current_nodes)

while True:
    for direction in instructions:
        steps = steps + 1

        for i in range(len(current_nodes)):
            if direction == "L":
                current_nodes[i] = nodes[current_nodes[i]][0]
            else:  # "R"
                current_nodes[i] = nodes[current_nodes[i]][1]

            if first_occurrence[i] == 0 and current_nodes[i][2] == "Z":
                first_occurrence[i] = steps

        if not 0 in first_occurrence:
            break
    else:
        continue
    break

print(lcm(*first_occurrence))
