import re

final_value = 0

def convert_str_to_int(str_number) -> int:
    if str_number.isnumeric():
            return int(str_number)
    if str_number == 'one':
            return 1
    if str_number == 'two':
            return 2
    if str_number == 'three':
            return 3
    if str_number == 'four':
            return 4
    if str_number == 'five':
            return 5
    if str_number == 'six':
            return 6
    if str_number == 'seven':
            return 7
    if str_number == 'eight':
            return 8
    if str_number == 'nine':
            return 9

with open("2023/1//input.txt", "r") as file:
    for line in file:
        numbers_in_line = re.findall(
            r"(?=(\d|one|two|three|four|five|six|seven|eight|nine))", line
        )
        two_digit_number = convert_str_to_int(numbers_in_line[0]) * 10 + convert_str_to_int(numbers_in_line[-1])
        final_value = final_value + two_digit_number

print(final_value)
