from functools import cmp_to_key
from enum import Enum
from collections import Counter

card_order = "J23456789TQKA"


class Type(Enum):
    NONE = 0
    HIGH_CARD = 1
    PAIR = 2
    TWO_PAIR = 3
    THREE_OF_KIND = 4
    FULL_HOUSE = 5
    FOUR_OF_KIND = 6
    FIVE_OF_KIND = 7


def get_type(cards) -> Type:
    counter = Counter(cards)
    number_of_jokers = counter["J"]
    counter.subtract({"J": number_of_jokers})
    multiple_occurrences = [i for i in counter.values() if i > 1]

    if (
        5 in multiple_occurrences
        or (4 in multiple_occurrences and number_of_jokers == 1)
        or (3 in multiple_occurrences and number_of_jokers == 2)
        or (2 in multiple_occurrences and number_of_jokers == 3)
        or number_of_jokers >= 4
    ):
        return Type.FIVE_OF_KIND

    if (
        4 in multiple_occurrences
        or (3 in multiple_occurrences and number_of_jokers == 1)
        or (2 in multiple_occurrences and number_of_jokers == 2)
        or number_of_jokers == 3
    ):
        return Type.FOUR_OF_KIND

    if all(x in multiple_occurrences for x in [3, 2]) or (
        multiple_occurrences == [2, 2] and number_of_jokers == 1
    ):
        return Type.FULL_HOUSE

    if (
        3 in multiple_occurrences
        or (2 in multiple_occurrences and number_of_jokers == 1)
        or number_of_jokers == 2
    ):
        return Type.THREE_OF_KIND

    if 2 in Counter(multiple_occurrences).values():
        return Type.TWO_PAIR

    if 2 in multiple_occurrences or number_of_jokers == 1:
        return Type.PAIR

    if len(multiple_occurrences) == 0:
        if card_order.find("".join(cards)) > -1:
            return Type.HIGH_CARD

        return Type.NONE


def cmp_cards(a, b):
    a_score = get_type(a["hand"]).value
    b_score = get_type(b["hand"]).value

    if a_score == b_score:
        for i in range(5):
            if a["hand"][i] != b["hand"][i]:
                return card_order.find(a["hand"][i]) - card_order.find(b["hand"][i])

    return a_score - b_score


hands = []

with open("2023/7/input.txt") as file:
    for line in file:
        data = line.split()
        hands.append({"hand": [i for i in data[0]], "bet": int(data[1])})

hands = sorted(hands, key=cmp_to_key(cmp_cards))

total_winning = 0
for i, hand in enumerate(hands):
    total_winning = total_winning + (i + 1) * hand["bet"]
print(total_winning)
