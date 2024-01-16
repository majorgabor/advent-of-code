
total = 0

def predict_next_value(values) -> int:

    if all(v == 0 for v in values):
        return 0

    return values[-1] + predict_next_value([j-i for i, j in zip(values[:-1], values[1:])])


with open("2023/9/input.txt", "r") as file:
    for line in file:
        values = [int(i) for i in line.split()]
        total = total + predict_next_value(values)

print(total)