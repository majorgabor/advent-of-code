#include <iostream>
#include <fstream>
#include <vector>
#include <algorithm>
#include <sstream>
#include <memory>

struct Range
{
    const uint start;
    const uint length;

    Range(const uint s, const uint l) : start(s), length(l){};
};

struct MapRange
{
    const uint destination_start;
    const uint source_start;
    const uint length;

    MapRange(const uint d, const uint s, const uint l) : destination_start(d), source_start(s), length(l){};
};

typedef std::shared_ptr<Range> Range_Ptr;
typedef std::vector<std::shared_ptr<Range>> Ranges;

typedef std::shared_ptr<MapRange> MapRange_Ptr;
typedef std::vector<std::shared_ptr<MapRange>> Map;
typedef std::vector<std::vector<std::shared_ptr<MapRange>>> Maps;

void readInputFile(const std::string &file_location, Ranges &seeds, Maps &maps);
void depthFirstResolver(const Maps &maps, const uint depth, const Range_Ptr &source_range, uint &global_minimum_location);
Ranges resolveRange(const Map &map, const Range_Ptr &source_range);
void sortMaps(Maps &maps);

int main()
{
    Ranges seeds;
    Maps maps(7);
    uint global_minimum_location = -1; // not pretty but should work

    readInputFile("input.txt", seeds, maps);
    // sortMaps(maps);

    for (auto &seed : seeds)
    {
        depthFirstResolver(maps, 0, seed, global_minimum_location);
    }
    std::cout << "global minimum: " << global_minimum_location << std::endl;

    return 0;
}

void readInputFile(const std::string &file_location, Ranges &seeds, Maps &maps)
{
    uint map_category_index = 0;
    std::vector<std::string> map_categories = {
        "seed-to-soil map:",
        "soil-to-fertilizer map:",
        "fertilizer-to-water map:",
        "water-to-light map:",
        "light-to-temperature map:",
        "temperature-to-humidity map:",
        "humidity-to-location map:"};

    std::ifstream file(file_location);
    if (file.is_open())
    {
        std::string line;
        if (std::getline(file, line))
        {
            // seeds
            line.erase(0, 7); // cut down 'seeds: '
            std::istringstream iss(line);
            std::vector<std::string> tokens;
            std::string token;
            while (std::getline(iss, token, ' '))
            {
                tokens.push_back(token);
            }
            for (uint i = 0; i < tokens.size(); i = i + 2)
            {
                seeds.push_back(std::make_shared<Range>(std::stoul(tokens[i]), std::stoul(tokens[i + 1])));
            }

            // maps
            while (std::getline(file, line))
            {
                if (line == "")
                {
                    std::getline(file, line);
                    auto it = std::find(map_categories.begin(), map_categories.end(), line);
                    if (it != map_categories.end())
                    {
                        map_category_index = std::distance(map_categories.begin(), it);
                    }
                }
                else
                {
                    std::istringstream iss(line);
                    uint d, s, l;
                    iss >> d >> s >> l;
                    maps[map_category_index].push_back(std::make_shared<MapRange>(d, s, l));
                }
            }
            file.close();
        }
    }
}

void sortMaps(Maps &maps)
{
    for (auto &map : maps)
    {
        std::sort(map.begin(), map.end(), [](const MapRange_Ptr &a, const MapRange_Ptr &b)
                  { return a->source_start < b->source_start; });
    }
}

void depthFirstResolver(const Maps &maps, const uint depth, const Range_Ptr &source_range, uint &global_minimum_location)
{
    auto resolved_ranges = resolveRange(maps[depth], source_range);
    if (depth == 6u)
    {
        for (auto &location_range : resolved_ranges)
        {
            if (location_range->start < global_minimum_location)
            {
                global_minimum_location = location_range->start;
            }
        }
    }
    else
    {
        const uint next_depth = depth + 1u;
        for (auto &resolved_range : resolved_ranges)
        {
            depthFirstResolver(maps, next_depth, resolved_range, global_minimum_location);
        }
    }
}

Ranges resolveRange(const Map &map, const Range_Ptr &source_range)
{
    Ranges resolved_ranges;

    for (const auto &row : map) // use that 'row' ordered
    {
        if (row->source_start <= source_range->start and source_range->start < (row->source_start + row->length))
        {
            const int offset = source_range->start - row->source_start;
            if ((source_range->start + source_range->length) < (row->source_start + row->length))
            {
                resolved_ranges.push_back(std::make_shared<Range>(row->destination_start + offset, source_range->length));
            }
            else
            {
                resolved_ranges.push_back(std::make_shared<Range>(row->destination_start + offset, row->length - offset));
            }
        }
        else if (source_range->start < row->source_start and row->source_start < (source_range->start + source_range->length))
        {
            const int offset = row->source_start - source_range->start;
            if ((source_range->start + source_range->length) < (row->source_start + row->length))
            {
                resolved_ranges.push_back(std::make_shared<Range>(row->destination_start, source_range->length - offset));
            }
            else
            {
                resolved_ranges.push_back(std::make_shared<Range>(row->destination_start, row->length));
            }
        }
    }

    // sort
    // std::sort(resolved_ranges.begin(), resolved_ranges.end(), [](const Range_Ptr &a, const Range_Ptr &b)
    //           { return a->start < b->start; });
    
    // look for unmapped ranges
    if (resolved_ranges.empty())
    {
        resolved_ranges.push_back(source_range);
    }

    return resolved_ranges;
}
