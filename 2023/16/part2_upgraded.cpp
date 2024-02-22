/**
 * Upgraded version of my original solutions for day 16.
 * 
 * After solving the puzzle myself I investigated if there would be a more efficient solution. There is!
 * from this sub reddit: https://www.reddit.com/r/adventofcode/comments/18jjpfk/2023_day_16_solutions/ I found https://github.com/charlescochran/aoc-2023/blob/main/day16.cpp this solution, which I took a look if it's better. It is!
 * Also I spiced up the parr2 with a second tread. 
*/

#include <fstream>
#include <vector>
#include <algorithm>
#include <set>
#include <map>
#include <chrono>
#include <future>

enum class Direction
{
    UPWARD,
    LEFTWARD,
    DOWNWARD,
    RIGHTWARD
};

std::map<char, std::map<Direction, std::vector<Direction>>> lookup_table{
    {'.', {{Direction::UPWARD, {Direction::UPWARD}}, {Direction::LEFTWARD, {Direction::LEFTWARD}}, {Direction::DOWNWARD, {Direction::DOWNWARD}}, {Direction::RIGHTWARD, {Direction::RIGHTWARD}}}},
    {'/', {{Direction::UPWARD, {Direction::RIGHTWARD}}, {Direction::LEFTWARD, {Direction::DOWNWARD}}, {Direction::DOWNWARD, {Direction::LEFTWARD}}, {Direction::RIGHTWARD, {Direction::UPWARD}}}},
    {'\\', {{Direction::UPWARD, {Direction::LEFTWARD}}, {Direction::LEFTWARD, {Direction::UPWARD}}, {Direction::DOWNWARD, {Direction::RIGHTWARD}}, {Direction::RIGHTWARD, {Direction::DOWNWARD}}}},
    {'|', {{Direction::UPWARD, {Direction::UPWARD}}, {Direction::LEFTWARD, {Direction::UPWARD, Direction::DOWNWARD}}, {Direction::DOWNWARD, {Direction::DOWNWARD}}, {Direction::RIGHTWARD, {Direction::UPWARD, Direction::DOWNWARD}}}},
    {'-', {{Direction::UPWARD, {Direction::LEFTWARD, Direction::RIGHTWARD}}, {Direction::LEFTWARD, {Direction::LEFTWARD}}, {Direction::DOWNWARD, {Direction::LEFTWARD, Direction::RIGHTWARD}}, {Direction::RIGHTWARD, {Direction::RIGHTWARD}}}}};

void expandLaserBeam(const int x, const int y, const Direction direction, const std::vector<std::string> &mat, std::set<std::tuple<int, int, Direction>> &energized_fields)
{
    if (energized_fields.find({x, y, direction}) == energized_fields.end())
    {
        if (0 <= x && x < mat.size() && 0 <= y && y < mat[0].size())
        {
            energized_fields.insert({x, y, direction});

            for (const Direction next_direction : lookup_table[mat[x][y]][direction])
            {
                switch (next_direction)
                {
                case Direction::UPWARD:
                    expandLaserBeam(x - 1, y, next_direction, mat, energized_fields);
                    break;
                case Direction::LEFTWARD:
                    expandLaserBeam(x, y - 1, next_direction, mat, energized_fields);
                    break;
                case Direction::DOWNWARD:
                    expandLaserBeam(x + 1, y, next_direction, mat, energized_fields);
                    break;
                case Direction::RIGHTWARD:
                    expandLaserBeam(x, y + 1, next_direction, mat, energized_fields);
                    break;
                }
            }
        }
    }
}

uint countEnergizedFieldsFrom(const int x, const int y, const Direction direction, std::vector<std::string> &mat)
{
    std::set<std::tuple<int, int, Direction>> energized_fields;

    expandLaserBeam(x, y, direction, mat, energized_fields);

    std::set<std::tuple<int, int>> unique_energized_fields;
    for (auto &field : energized_fields)
    {
        unique_energized_fields.insert({std::get<0>(field), std::get<1>(field)});
    }

    return unique_energized_fields.size();
}

int main()
{
    std::ifstream file("2023/16/input.txt");
    if (!file.is_open())
    {
        printf("Unable to open file!");
        return 1;
    }

    std::vector<std::string> mat;
    for (std::string line; std::getline(file, line);)
    {
        mat.push_back(line);
    }

    auto start = std::chrono::high_resolution_clock::now();

    std::future<uint> futureResult = std::async(std::launch::async, [&]()
                                                {
                                                   uint max_energized_fields = 0u;
                                                   for (uint i = 0; i < mat.size(); i++)
                                                   {
                                                       max_energized_fields = std::max(max_energized_fields, countEnergizedFieldsFrom(i, 0, Direction::RIGHTWARD, mat));
                                                       max_energized_fields = std::max(max_energized_fields, countEnergizedFieldsFrom(i, (mat[0].size() - 1), Direction::LEFTWARD, mat));
                                                   }
                                                   return max_energized_fields; });

    uint max_energized_fields = 0u;
    for (uint i = 0; i < mat[0].size(); i++)
    {
        max_energized_fields = std::max(max_energized_fields, countEnergizedFieldsFrom(0, i, Direction::DOWNWARD, mat));
        max_energized_fields = std::max(max_energized_fields, countEnergizedFieldsFrom((mat.size() - 1), i, Direction::UPWARD, mat));
    }

    max_energized_fields = std::max(max_energized_fields, futureResult.get());

    auto end = std::chrono::high_resolution_clock::now();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    printf("Clock time: %f seconds, (%ldms)\n", duration / 1e+6, duration);

    printf("%u\n", max_energized_fields);

    return 0;
}