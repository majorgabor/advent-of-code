surface = []


def get_first_steps(from_index) -> list:
    next_positions = []
    possible_s_values = {"J", "|", "F", "-", "7", "L"}

    if surface[from_index[0] - 1][from_index[1]] in ["7", "|", "F"]:
        next_positions.append((from_index[0] - 1, from_index[1]))
        possible_s_values = possible_s_values.intersection({"J", "|", "L"})

    if surface[from_index[0] + 1][from_index[1]] in ["J", "|", "L"]:
        next_positions.append((from_index[0] + 1, from_index[1]))
        possible_s_values = possible_s_values.intersection({"7", "|", "F"})

    if surface[from_index[0]][from_index[1] + 1] in ["-", "J", "7"]:
        next_positions.append((from_index[0], from_index[1] + 1))
        possible_s_values = possible_s_values.intersection({"L", "F", "-"})

    if surface[from_index[0]][from_index[1] - 1] in ["L", "F", "-"]:
        next_positions.append((from_index[0], from_index[1] - 1))
        possible_s_values = possible_s_values.intersection({"-", "J", "7"})

    (surface[from_index[0]][from_index[1]],) = possible_s_values

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


def is_east_west_pipe(index) -> bool:
    return surface[index[0]][index[1]] != "|"


def is_enclosed(tile_index, walls):
    moving_index = tile_index[0]
    cross = 0

    while moving_index > -1:
        if (moving_index, tile_index[1]) in walls:
            if surface[moving_index][tile_index[1]] == "J":
                moving_index = moving_index - 1
                while surface[moving_index][tile_index[1]] == "|":
                    moving_index = moving_index - 1
                if surface[moving_index][tile_index[1]] == "F":
                    cross = cross + 1

            elif surface[moving_index][tile_index[1]] == "L":
                moving_index = moving_index - 1
                while surface[moving_index][tile_index[1]] == "|":
                    moving_index = moving_index - 1
                if surface[moving_index][tile_index[1]] == "7":
                    cross = cross + 1

            elif surface[moving_index][tile_index[1]] == "-":
                cross = cross + 1

        moving_index = moving_index - 1

    return cross % 2 == 1


with open("2023/10/input.txt", "r") as file:
    for i, line in enumerate(file):
        pipes = list(line)

        if "S" in pipes:
            start_index = (i, pipes.index("S"))

        surface.append(pipes)

position_one, position_two = get_first_steps(start_index)
prev_position_one, prev_position_two = start_index, start_index


walls = {start_index, position_one, position_two}
while position_one != position_two:
    # one
    position_one_next = get_next_step(position_one, prev_position_one)
    walls.add(position_one_next)
    prev_position_one = position_one
    position_one = position_one_next
    # two
    position_two_next = get_next_step(position_two, prev_position_two)
    walls.add(position_two_next)
    prev_position_two = position_two
    position_two = position_two_next

enclosed = 0

for i in range(len(surface)):
    for j in range(len(surface[i])):
        if (i, j) not in walls:
            if is_enclosed((i, j), walls):
                enclosed = enclosed + 1

print(enclosed)
