from itertools import combinations


def calc_dist(a, b) -> int:
    return abs(a[0] - b[0]) + abs(a[1] - b[1])


universes = []
with open("2023/11/input.txt", "r") as file:
    for row, line in enumerate(file):
        for col in [col for col, x in enumerate(list(line)) if x == "#"]:
            universes.append((row, col))

# extend universe
rows_with_universe = [x[0] for x in universes]
cols_with_universe = [x[1] for x in universes]
empty_rows = set(range(max(rows_with_universe))) - set(rows_with_universe)
empty_cols = set(range(max(cols_with_universe))) - set(cols_with_universe)
expanded_universe = [
    (
        universe[0] + len([i for i in empty_rows if i < universe[0]]),
        universe[1] + len([i for i in empty_cols if i < universe[1]]),
    )
    for universe in universes
]

total_dist = 0
for pair in combinations(expanded_universe, r=2):
    total_dist = total_dist + calc_dist(*pair)

print(total_dist)
