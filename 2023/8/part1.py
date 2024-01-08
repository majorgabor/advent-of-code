nodes = {}

with open("2023/8/input.txt") as file:
    instructions = file.readline()[:-1]
    file.readline() # '\n'
    for line in file:
        data = line.split()
        nodes[data[0]] = (data[2][1:-1], data[3][:-1])

steps = 0
current_node = 'AAA'

while True:
    for direction in instructions:

        if direction == "L":
            current_node = nodes[current_node][0]
        else: #"R"
            current_node = nodes[current_node][1]
        
        steps = steps + 1

        if current_node == "ZZZ":
            break
    else:
        continue
    break

print(steps)