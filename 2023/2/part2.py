import re

sum_of_power = 0


def look_for_min_req_colors(single_color_cubes_in_hand, min_req_colors) -> bool:
    if min_req_colors[single_color_cubes_in_hand[1]] < int(
        single_color_cubes_in_hand[0]
    ):
        min_req_colors[single_color_cubes_in_hand[1]] = int(
            single_color_cubes_in_hand[0]
        )


def check_pull_from_bag(cubes_in_hand, min_req_colors) -> None:
    for single_color_cubes_in_hand in re.findall(
        r"(\d+) (red|green|blue)", cubes_in_hand
    ):
        look_for_min_req_colors(single_color_cubes_in_hand, min_req_colors)


def check_game(game_records, min_req_colors) -> None:
    for record in game_records.split("; "):
        check_pull_from_bag(record, min_req_colors)


with open("2023/2/input.txt", "r") as file:
    for line in file:
        min_req_colors = {"red": 0, "green": 0, "blue": 0}

        check_game(line.split(": ")[1], min_req_colors)

        sum_of_power = (
            sum_of_power
            + min_req_colors["red"] * min_req_colors["green"] * min_req_colors["blue"]
        )

print(sum_of_power)
