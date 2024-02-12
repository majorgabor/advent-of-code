"""
with some help from https://www.reddit.com/media?url=https%3A%2F%2Fi.redd.it%2F2023-day-12-part-2-this-image-helped-a-few-people-to-solve-v0-46y3l042m06c1.png%3Fs%3Dd944fffd4f3b4f73ac26c336af377d35bd364e9f
"""
def get_next_state(next_character, damaged_group, state) -> list:
    next_states = []

    if next_character == "?":
        if state[2] < damaged_group[state[1]]:
            next_states.append((state[0] + 1, state[1], state[2] + 1))  # bad

            if state[2] == 0:
                next_states.append((state[0] + 1, state[1], 0))  # good

        elif state[2] == damaged_group[state[1]]:
            next_states.append((state[0] + 1, state[1] + 1, 0))  # good

    elif next_character == "#":
        if state[2] < damaged_group[state[1]]:
            next_states.append((state[0] + 1, state[1], state[2] + 1))  # bad

    else:  # next_character == ".":
        if state[2] == 0:
            next_states.append((state[0] + 1, state[1], 0))  # good

        elif state[2] == damaged_group[state[1]]:
            next_states.append((state[0] + 1, state[1] + 1, 0))  # good

    return next_states


def valid_permutation_counter(spring_row, damaged_group, state, memory) -> int:

    if state in memory:
        return memory[state]

    iteration = state[0]
    group_counter = state[1]
    amount = state[2]

    # is it finished
    if iteration == len(spring_row):
        # is it in a valid state
        if (group_counter == len(damaged_group) and amount == 0) or (
            group_counter == len(damaged_group) - 1 and amount == damaged_group[-1]
        ):
            return 1
        # invalid state
        return 0

    # is all 'bad' found
    if group_counter == len(damaged_group) and amount == 0:
        if all(x == "." or x == "?" for x in spring_row[iteration:]):
            return 1
        else:
            return 0
    # is current state invalid
    if group_counter >= len(damaged_group) or amount > damaged_group[group_counter]:
        return 0

    # -------------------------------
    # take it to memory
    memory[state] = 0
    # explore next state(s)
    for next_state in get_next_state(spring_row[iteration], damaged_group, state):
        memory[state] += valid_permutation_counter(
            spring_row, damaged_group, next_state, memory
        )

    return memory[state]

total = 0

with open("2023/12/input.txt", "r") as file:
    for line in file:
        d = line.split()
        spring_row = d[0]
        damaged_group = [int(i) for i in d[1].split(",")] * 5
        for _ in range(4):
            spring_row = spring_row + "?" + d[0]

        total += valid_permutation_counter(spring_row, damaged_group, (0, 0, 0), dict())

print(total)
