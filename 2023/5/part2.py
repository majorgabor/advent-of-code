from enum import Enum


class Maps(Enum):
    SEED_TO_SOIL = 1
    SOIL_TO_FERTILIZER = 2
    FERTILIZER_TO_WATER = 3
    WATER_TO_LIGHT = 4
    LIGHT_TO_TEMPERATURE = 5
    TEMPERATURE_TO_HUMIDITY = 6
    HUMIDITY_TO_LOCATION = 7


def resolve_map_range(map, input_range) -> int:
    resolved_range = []

    for row in map:
        # if map range starts before input_range
        if row[1] <= input_range[0] and input_range[0] < (row[1] + row[2]):
            #
            offset = input_range[0] - row[1]

            # if map range ends after input_range
            if (input_range[0] + input_range[1]) < (row[1] + row[2]):
                resolved_range.append((row[0] + offset, input_range[1]))
            # else map range ends before input_range
            else:
                resolved_range.append((row[0] + offset, row[2] - offset))

        # elif map range starts after input_range and before input_range ends
        elif input_range[0] < row[1] and row[1] < (input_range[0] + input_range[1]):
            #
            offset = row[1] - input_range[0]
            # if map range ends after input_range
            if (input_range[0] + input_range[1]) < (row[1] + row[2]):
                resolved_range.append((row[0], input_range[1] - offset))
            # else map range ends before input_range
            else:
                resolved_range.append((row[0], row[2]))

    # if unmapped
    if resolved_range == []:
        resolved_range.append(input_range)

    return resolved_range


min_location = 9223372036854775807  # 2**63-1 (hopefully will be lower)


def depth_firs_resolver(depth, range_to_resolve):
    global min_location
    next_category_ranges = resolve_map_range(maps[Maps(depth).name], range_to_resolve)
    if depth == Maps.HUMIDITY_TO_LOCATION.value:
        for location_range in next_category_ranges:
            if location_range[0] < min_location:
                min_location = location_range[0]
                print(f"new minimum: {min_location}")
    else:
        next_depth = depth + 1
        for next_category_range in next_category_ranges:
            depth_firs_resolver(next_depth, next_category_range)


seed_ranges = []
maps = {}
for enum in Maps:
    maps[enum.name] = []

with open("2023/5/input.txt", "r") as file:
    currently_reading_map = ""
    for i, line in enumerate(file):
        if line == "\n":
            continue
        if i == 0:
            seeds_str = line[6:-1].split()
            for j in range(0, len(seeds_str), 2):
                seed_ranges.append((int(seeds_str[j]), int(seeds_str[j + 1])))
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

for seed_range in seed_ranges:
    depth_firs_resolver(1, seed_range)


print(min_location)
