from math import sqrt, ceil, floor

with open("2023/6/input.txt") as file:
    times = [int(i) for i in file.readline().split()[1:]]
    distances = [int(i) for i in file.readline().split()[1:]]


number_of_way_to_beat_record = 1

for time, distance in zip(times, distances):
    # calculate discriminant
    discriminant = time**2 - 4 * distance

    # if discriminant == 0:
    #     # one way win
    #     pass
    if discriminant > 0:
        # optimal time frame to press button
        number_of_way_to_beat_record = number_of_way_to_beat_record * (
            floor((time + sqrt(discriminant)) / 2)
            - ceil((time - sqrt(discriminant)) / 2)
            + 1
        )


print(number_of_way_to_beat_record)
