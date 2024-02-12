import time


total = 0

def decision_tree(spring_row, damaged_group) -> int:
    if "?" in spring_row:
        return decision_tree(
            spring_row.replace("?", "#", 1), damaged_group
        ) + decision_tree(spring_row.replace("?", ".", 1), damaged_group)

    else:
        return int(damaged_group == [len(x) for x in spring_row.split(".") if "#" in x])

start = time.time()

with open("2023/12/input.txt", "r") as file:
    for line in file:
        d = line.split()
        total = total + decision_tree(d[0], [int(i) for i in d[1].split(",")])

print(total)
print(time.time() - start)
