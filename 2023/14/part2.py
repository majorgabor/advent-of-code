def unifier(seq):
    seen = set()
    seen_add = seen.add
    return [x for x in seq if not (x in seen or seen_add(x))]


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


def tilt_to_west(mat) -> list:
    rows = len(mat)
    cols = len(mat[0])

    for i in range(rows):
        western_reachable_spot = 0
        for j in range(cols):
            if mat[i][j] == "#":
                western_reachable_spot = j + 1
            if mat[i][j] == "O":
                if j == western_reachable_spot:
                    western_reachable_spot = j + 1
                    continue
                mat[i][western_reachable_spot] = "O"
                mat[i][j] = "."
                western_reachable_spot += 1
                while (
                    western_reachable_spot < rows
                    and mat[i][western_reachable_spot] == "#"
                ):
                    western_reachable_spot += 1
    return mat


def tilt_to_south(mat) -> list:
    rows = len(mat)
    cols = len(mat[0])

    for i in range(cols):
        j = i
        southeast_reachable_spot = rows - 1
        for j in range(rows - 1, -1, -1):
            if mat[j][i] == "#":
                southeast_reachable_spot = j - 1
            if mat[j][i] == "O":
                if j == southeast_reachable_spot:
                    southeast_reachable_spot = j - 1
                    continue
                mat[southeast_reachable_spot][i] = "O"
                mat[j][i] = "."
                southeast_reachable_spot -= 1
                while (
                    southeast_reachable_spot > 0
                    and mat[southeast_reachable_spot][i] == "#"
                ):
                    southeast_reachable_spot -= 1
    return mat


def tilt_to_east(mat) -> list:
    rows = len(mat)
    cols = len(mat[0])

    for i in range(rows):
        eastern_reachable_spot = cols - 1
        for j in range(cols - 1, -1, -1):
            if mat[i][j] == "#":
                eastern_reachable_spot = j - 1
            if mat[i][j] == "O":
                if j == eastern_reachable_spot:
                    eastern_reachable_spot = j - 1
                    continue
                mat[i][eastern_reachable_spot] = "O"
                mat[i][j] = "."
                eastern_reachable_spot -= 1
                while (
                    eastern_reachable_spot > 0 and mat[i][eastern_reachable_spot] == "#"
                ):
                    eastern_reachable_spot -= 1
    return mat


results = dict()
with open("2023/14/input.txt") as file:
    mat = [list(line.strip()) for line in file]
    for i in range(1000):
        mat = tilt_to_east(tilt_to_south(tilt_to_west(tilt_to_north(mat))))
        if i > 171: # where it starts to 'normalize' the pattern
            load = count_total_load(mat)
            if load in results:
                results[load].append(i)                
            else:
                results[load] = [i]

for k, v in results.items():
    if len(v) > 1:
        pattern = unifier([t - s for s, t in zip(v[:-1], v[1:])])
        n = (1000000000 - 1 - (v[0])) % sum(pattern)
        if n == 0:
            print(k)
        else:
            for i in range(1, len(pattern)+1):
                if n == sum(pattern[0:i]):
                    print(k)
                    break