
pieces = [0] * 220

with open("2023/4/input.txt", "r") as lottery_cards:
    wins = []
    for i, lottery_card in enumerate(lottery_cards):
        numbers = (lottery_card.split(": ")[1]).split(" | ")
        winning_numbers = list(map(int, numbers[0].split()))
        given_numbers = list(map(int, numbers[1].split()))
        wins.append(len(set(winning_numbers) & set(given_numbers)))
        
        pieces[i] = 1
        for j in range(i):
            if i - j <= wins[j]:
                pieces[i] = pieces[i] + pieces[j]

print(sum(pieces))