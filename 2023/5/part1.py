from enum import Enum


class Maps(Enum):
    SEED_TO_SOIL = 1
    SOIL_TO_FERTILIZER = 2
    FERTILIZER_TO_WATER = 3
    WATER_TO_LIGHT = 4
    LIGHT_TO_TEMPERATURE = 5
    TEMPERATURE_TO_HUMIDITY = 6
    HUMIDITY_TO_LOCATION = 7


def resolve_map(map, input_code) -> int:
    for row in map:
        if row[1] <= input_code and input_code < (row[1] + row[2]):
            return row[0] + (input_code - row[1])
    return input_code


seeds = []
maps = {}
for enum in Maps:
    maps[enum.name] = []

with open("2023/5/input.txt", "r") as file:
    currently_reading_map = ""
    for i, line in enumerate(file):
        if line == "\n":
            continue
        if i == 0:
            for seed in line[6:-1].split():
                seeds.append(int(seed))
            continue
        if line == "seed-to-soil map:\n":
            currently_reading_map = maps[Maps.SEED_TO_SOIL.name]
            continue
        if line == "soil-to-fertilizer map:\n":
            currently_reading_map = maps[Maps.SOIL_TO_FERTILIZER.name]
            continue
        if line == "fertilizer-to-water map:\n":
            currently_reading_map = maps[Maps.FERTILIZER_TO_WATER.name]
            continue
        if line == "water-to-light map:\n":
            currently_reading_map = maps[Maps.WATER_TO_LIGHT.name]
            continue
        if line == "light-to-temperature map:\n":
            currently_reading_map = maps[Maps.LIGHT_TO_TEMPERATURE.name]
            continue
        if line == "temperature-to-humidity map:\n":
            currently_reading_map = maps[Maps.TEMPERATURE_TO_HUMIDITY.name]
            continue
        if line == "humidity-to-location map:\n":
            currently_reading_map = maps[Maps.HUMIDITY_TO_LOCATION.name]
            continue

        row = []
        for number in line.split():
            row.append(int(number))
        currently_reading_map.append(row)

locations = []
for seed in seeds:
    input_code = seed
    for map in (Maps):
        input_code = resolve_map(maps[map.name], input_code)
    locations.append(input_code)

print(min(locations))