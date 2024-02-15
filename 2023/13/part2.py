def are_rows_equal(a, b) -> tuple:
    smudge = 0
    for i, j in zip(a, b):
        if i != j:
            smudge += 1
            if smudge > 1:
                return False, 2
    return True, smudge


def find_mirroring_point(mat) -> int:
    rows = len(mat)

    # try pairs
    for i in range(rows - 1):
        smudges = 0
        are_equal, smudge = are_rows_equal(mat[i], mat[i + 1])
        smudges += smudge
        if are_equal:
            # test 'mirroring'
            for j in range(1, min(i + 1, rows - (i + 1))):
                are_equal, smudge = are_rows_equal(mat[i - j], mat[i + 1 + j])
                smudges += smudge
                if not are_equal or smudges > 1:
                    break
            else:
                if smudges == 1:
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
                mat_t = [
                    [mat[j][i] for j in range(len(mat))] for i in range(len(mat[0]))
                ]
                total += find_mirroring_point(mat_t)
            mat.clear()
        else:
            mat.append(list(line.strip()))

print(total)
