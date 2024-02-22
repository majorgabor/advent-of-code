#include <fstream>
#include <vector>
#include <algorithm>
#include <set>

enum class Direction
{
    UPWARD,
    LEFTWARD,
    DOWNWARD,
    RIGHTWARD
};

inline bool isFieldEquals(const std::tuple<uint, uint, Direction> &field, const uint x, const uint y, const Direction direction)
{
    return std::get<0>(field) == x && std::get<1>(field) == y && (std::get<2>(field) == direction || std::get<2>(field) == static_cast<Direction>(static_cast<int>(direction) + 2));
}

bool hasBeenExpandedFromDirection(const uint x, const uint y, const Direction direction, const std::set<std::tuple<uint, uint, Direction>> &energized_fields)
{
    return std::find_if(energized_fields.begin(), energized_fields.end(), std::bind(isFieldEquals, std::placeholders::_1, x, y, direction)) != energized_fields.end();
}

Direction getReflection(const Direction direction, const char mirror)
{
    switch (direction)
    {
    case Direction::UPWARD:
        return (mirror == '/') ? Direction::RIGHTWARD : Direction::LEFTWARD;
    case Direction::LEFTWARD:
        return (mirror == '/') ? Direction::DOWNWARD : Direction::UPWARD;
    case Direction::DOWNWARD:
        return (mirror == '/') ? Direction::LEFTWARD : Direction::RIGHTWARD;
    case Direction::RIGHTWARD:
        return (mirror == '/') ? Direction::UPWARD : Direction::DOWNWARD;
    }
}

bool setNextXY(const uint x, const uint y, const Direction direction, uint &next_x, uint &next_y, const uint x_max, const uint y_max)
{
    next_x = x;
    next_y = y;
    switch (direction)
    {
    case Direction::UPWARD:
        return (next_x-- > 0);
    case Direction::LEFTWARD:
        return (next_y-- > 0);
    case Direction::DOWNWARD:
        return (++next_x < x_max);
    case Direction::RIGHTWARD:
        return (++next_y < y_max);
    }
}

void expandLaserBeam(const uint x, const uint y, const Direction direction, const std::vector<std::string> &mat, std::set<std::tuple<uint, uint, Direction>> &energized_fields)
{
    const bool has_been_explored = hasBeenExpandedFromDirection(x, y, direction, energized_fields);
    if (!has_been_explored || (has_been_explored && (mat[x][y] == '/' || mat[x][y] == '\\')))
    {
        energized_fields.insert({x, y, direction});

        uint next_x, next_y;
        if (mat[x][y] == '.')
        {
            if (setNextXY(x, y, direction, next_x, next_y, mat.size(), mat[0].size()))
                expandLaserBeam(next_x, next_y, direction, mat, energized_fields);
        }
        else if (mat[x][y] == '/' || mat[x][y] == '\\')
        {
            Direction next_direction = getReflection(direction, mat[x][y]);
            if (setNextXY(x, y, next_direction, next_x, next_y, mat.size(), mat[0].size()))
                expandLaserBeam(next_x, next_y, next_direction, mat, energized_fields);
        }
        else
        {
            const bool left_to_right = (direction == Direction::LEFTWARD || direction == Direction::RIGHTWARD);
            if (mat[x][y] == '|' && left_to_right)
            {
                for (auto next_direction : {Direction::UPWARD, Direction::DOWNWARD})
                {
                    if (setNextXY(x, y, next_direction, next_x, next_y, mat.size(), mat[0].size()))
                        expandLaserBeam(next_x, next_y, next_direction, mat, energized_fields);
                }
            }
            else if (mat[x][y] == '-' && !left_to_right)
            {
                for (auto next_direction : {Direction::LEFTWARD, Direction::RIGHTWARD})
                {
                    if (setNextXY(x, y, next_direction, next_x, next_y, mat.size(), mat[0].size()))
                        expandLaserBeam(next_x, next_y, next_direction, mat, energized_fields);
                }
            }
            else
            {
                if (setNextXY(x, y, direction, next_x, next_y, mat.size(), mat[0].size()))
                    expandLaserBeam(next_x, next_y, direction, mat, energized_fields);
            }
        }
    }
}

uint countEnergizedFieldsFrom(const uint x, const uint y, const Direction direction, const std::vector<std::string> &mat)
{
    std::set<std::tuple<uint, uint, Direction>> energized_fields;

    expandLaserBeam(x, y, direction, mat, energized_fields);

    std::set<std::tuple<uint, uint>> unique_energized_fields;
    for (auto field : energized_fields)
    {
        unique_energized_fields.insert({std::get<0>(field), std::get<1>(field)});
    }
    return unique_energized_fields.size();
}

int main()
{
    std::fstream file("2023/16/input.txt", std::ios_base::in);
    if (!file.is_open())
    {
        printf("Unable to open file!");
        return 1;
    }

    std::vector<std::string> mat;
    std::string line;
    while (std::getline(file, line))
    {
        mat.push_back(line);
    }

    uint max_energized_fields = 0u;

    uint result = 0u;

    for (uint i = 0; i < mat.size(); i++)
    {
        result = countEnergizedFieldsFrom(i, 0, Direction::RIGHTWARD, mat);
        max_energized_fields = result > max_energized_fields ? result : max_energized_fields;
        result = countEnergizedFieldsFrom(i, (mat[0].size() - 1), Direction::LEFTWARD, mat);
        max_energized_fields = result > max_energized_fields ? result : max_energized_fields;
    }
    printf("Half way there\n");
    for (uint i = 0; i < mat[0].size(); i++)
    {
        result = countEnergizedFieldsFrom(0, i, Direction::DOWNWARD, mat);
        max_energized_fields = result > max_energized_fields ? result : max_energized_fields;
        result = countEnergizedFieldsFrom((mat.size() - 1), i, Direction::UPWARD, mat);
        max_energized_fields = result > max_energized_fields ? result : max_energized_fields;
    }

    printf("%u\n", max_energized_fields);

    return 0;
}