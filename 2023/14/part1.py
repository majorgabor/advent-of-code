def count_total_load(mat) -> int:
    row = [(0, 0)] * len(mat[0])
    for i in range(len(mat)):
        for j in range(len(mat[i])):
            rocks = row[j][0] + 1 if mat[i][j] == "O" else row[j][0]
            row[j] = (rocks, row[j][1] + rocks)
    return sum([pair[1] for pair in row])


def tilt_to_north(mat) -> list:
    rows = len(mat)
    cols = len(mat[0])

    for i in range(cols):
        j = i
        northeast_reachable_spot = 0
        for j in range(rows):
            if mat[j][i] == "#":
                northeast_reachable_spot = j + 1
            if mat[j][i] == "O":
                if j == northeast_reachable_spot:
                    northeast_reachable_spot = j + 1
                    continue
                mat[northeast_reachable_spot][i] = "O"
                mat[j][i] = "."
                northeast_reachable_spot += 1
                while (
                    northeast_reachable_spot < rows
                    and mat[northeast_reachable_spot][i] == "#"
                ):
                    northeast_reachable_spot += 1
    return mat


with open("2023/14/input.txt") as file:
    mat = [list(line.strip()) for line in file]
    print(count_total_load(tilt_to_north(mat)))
