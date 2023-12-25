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


sum_of_gear_ratios = 0

with open("2023/3/input.txt", "r") as motor_schematics:
    text = motor_schematics.read()
    number_positions = [i for i in re.finditer(r"\d+", text)]
    gear_positions = set(
        [i.start() for i in re.finditer(r"[\*]", text)]
    )

    gears = {}

    for number in number_positions:
        gear_pos = get_adjunct_positions(number.span()[0], number.span()[1]) & gear_positions
        if gear_pos:
            gear_pos_idx_str = str(gear_pos.pop())
            if gear_pos_idx_str in gears:
                # add to sum
                sum_of_gear_ratios = sum_of_gear_ratios + gears[gear_pos_idx_str] * int(number.group())
                del gears[gear_pos_idx_str] # just to clean a bit
            else:
                # save number belonging to gear
                gears[gear_pos_idx_str] = int(number.group())
            

print(sum_of_gear_ratios)
