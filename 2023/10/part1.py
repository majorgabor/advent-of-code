surface = []


def get_first_steps(from_index) -> list:
    next_positions = []

    if surface[from_index[0] - 1][from_index[1]] in ["7", "|", "F"]:
        next_positions.append((from_index[0] - 1, from_index[1]))

    if surface[from_index[0] + 1][from_index[1]] in ["J", "|", "L"]:
        next_positions.append((from_index[0] + 1, from_index[1]))

    if surface[from_index[0]][from_index[1] + 1] in ["-", "J", "7"]:
        next_positions.append((from_index[0], from_index[1] + 1))

    if surface[from_index[0]][from_index[1] - 1] in ["L", "F", "-"]:
        next_positions.append((from_index[0], from_index[1] - 1))

    return next_positions


def get_next_step(from_index, previous_index) -> tuple:
    # | is a vertical pipe connecting north and south.
    if surface[from_index[0]][from_index[1]] == "|":
        if previous_index[0] < from_index[0]:
            return (from_index[0] + 1, from_index[1])
        else:
            return (from_index[0] - 1, from_index[1])
    # - is a horizontal pipe connecting east and west.
    if surface[from_index[0]][from_index[1]] == "-":
        if previous_index[1] < from_index[1]:
            return (from_index[0], from_index[1] + 1)
        else:
            return (from_index[0], from_index[1] - 1)
    # L is a 90-degree bend connecting north and east.
    if surface[from_index[0]][from_index[1]] == "L":
        if previous_index[1] == from_index[1]:
            return (from_index[0], from_index[1] + 1)
        else:
            return (from_index[0] - 1, from_index[1])
    # J is a 90-degree bend connecting north and west.
    if surface[from_index[0]][from_index[1]] == "J":
        if previous_index[1] == from_index[1]:
            return (from_index[0], from_index[1] - 1)
        else:
            return (from_index[0] - 1, from_index[1])
    # 7 is a 90-degree bend connecting south and west.
    if surface[from_index[0]][from_index[1]] == "7":
        if previous_index[1] == from_index[1]:
            return (from_index[0], from_index[1] - 1)
        else:
            return (from_index[0] + 1, from_index[1])
    # F is a 90-degree bend connecting south and east.
    if surface[from_index[0]][from_index[1]] == "F":
        if previous_index[1] == from_index[1]:
            return (from_index[0], from_index[1] + 1)
        else:
            return (from_index[0] + 1, from_index[1])


with open("2023/10/input.txt", "r") as file:
    for i, line in enumerate(file):
        pipes = list(line)

        if "S" in pipes:
            start_index = (i, pipes.index("S"))

        surface.append(pipes)

steps = 1
position_one, position_two = get_first_steps(start_index)
prev_position_one, prev_position_two = start_index, start_index


while position_one != position_two:
    steps = steps + 1
    # one
    position_one_next = get_next_step(position_one, prev_position_one)
    prev_position_one = position_one
    position_one = position_one_next
    # two
    position_two_next = get_next_step(position_two, prev_position_two)
    prev_position_two = position_two
    position_two = position_two_next

print(steps)
