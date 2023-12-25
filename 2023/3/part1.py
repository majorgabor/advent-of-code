import re


def get_adjunct_positions(start_idx, end_idx):
    res = set()

    if start_idx % 141 != 0:
        res.add(start_idx - 142)
        res.add(start_idx - 1)
        res.add(start_idx + 140)

    if end_idx % 141 != 140:
        res.add(end_idx - 141)
        res.add(end_idx)
        res.add(end_idx + 141)

    i = start_idx
    while i < end_idx:
        res.add(i - 141)  # add positions above
        res.add(i + 141)  # add positions below
        i = i + 1

    return res


sum_of_part_numbers = 0

with open("2023/3/input.txt", "r") as motor_schematics:
    text = motor_schematics.read()
    number_positions = [i for i in re.finditer(r"\d+", text)]
    symbols_positions = set([i.start() for i in re.finditer(r"[@#\$%&=\*+\-/]", text)])

    for number in number_positions:
        if (
            get_adjunct_positions(number.span()[0], number.span()[1])
            & symbols_positions
        ):
            sum_of_part_numbers = sum_of_part_numbers + int(number.group())


print(sum_of_part_numbers)
