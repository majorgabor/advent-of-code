import re

final_value = 0

with open("2023/1/input.txt", "r") as file:
    for line in file:
        numbers_in_line = re.findall(
            r"\d", line
        )
        two_digit_number = int(numbers_in_line[0]) * 10 + int(numbers_in_line[-1])
        final_value = final_value + two_digit_number

print(final_value)
