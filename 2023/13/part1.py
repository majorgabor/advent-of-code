def find_mirroring_point(mat) -> int:
    rows = len(mat)
    # try pairs
    for i in range(rows - 1):
        if mat[i] == mat[i + 1]:
            # test 'mirroring'
            for j in range(1, min(i+1, rows - (i + 1))):
                if mat[i - j] != mat[i + 1 + j]:
                    break
            else:
                return i + 1
    return 0

total = 0

with open("2023/13/input.txt") as file:
    mat = []
    for line in file:
        if line == "\n":
            row = find_mirroring_point(mat) * 100
            if row > 0:
                total += row
            else:
                mat_t = [[mat[j][i] for j in range(len(mat))] for i in range(len(mat[0]))]
                total += find_mirroring_point(mat_t)
            mat.clear()
        else:
            mat.append(list(line.strip()))

print(total)
