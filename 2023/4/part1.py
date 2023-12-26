points = 0

with open("2023/4/input.txt", "r") as lottery_cards:
    for lottery_card in lottery_cards:
        numbers = (lottery_card.split(": ")[1]).split(" | ")
        winning_numbers = list(map(int, numbers[0].split()))
        given_numbers = list(map(int, numbers[1].split()))

        number_of_matches = len(set(winning_numbers) & set(given_numbers))

        if number_of_matches > 0:
            points = points + pow(2, number_of_matches - 1)

print(points)
