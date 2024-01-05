from math import sqrt, ceil, floor

with open("2023/6/input.txt") as file:
    time = int("".join(file.readline().split()[1:]))
    distance = int("".join(file.readline().split()[1:]))


# calculate discriminant
discriminant = time**2 - 4 * distance

if discriminant > 0:
    # optimal time frame to press button
    number_of_way_to_beat_record = (
        floor((time + sqrt(discriminant)) / 2)
        - ceil((time - sqrt(discriminant)) / 2)
        + 1
    )


print(number_of_way_to_beat_record)
