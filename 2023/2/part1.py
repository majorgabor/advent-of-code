import re

max_colors = {"red": 12, "green": 13, "blue": 14}

sum_ids = 0


def are_there_enough_cubes_of_color(single_color_cubes_in_hand) -> bool:
    return int(single_color_cubes_in_hand[0]) <= max_colors[single_color_cubes_in_hand[1]]


def check_pull_from_bag(cubes_in_hand) -> bool:
    for single_color_cubes_in_hand in re.findall(
        r"(\d+) (red|green|blue)", cubes_in_hand
    ):
        if not are_there_enough_cubes_of_color(single_color_cubes_in_hand):
            return False
    return True

def check_game(game_records) -> bool:
    for record in game_records.split("; "):
        if not check_pull_from_bag(record):
            return False
    return True

with open("2023/2/input.txt", "r") as file:
    for line in file:
        if check_game(line.split(": ")[1]):
            # get group ID
            sum_ids = sum_ids + int(re.search(r"\d+", line).group(0))

print(sum_ids)