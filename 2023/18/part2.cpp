#include <fstream>
#include <sstream>
#include <vector>
#include <tuple>
#include <map>

struct InputData
{
    char direction;
    int length;
    std::string color_code;
};

enum class VertexType
{
    SLASH = 0,
    BACK_SLASH = 1,
};

struct Vertex
{
    std::pair<int64_t, int64_t> position;
    VertexType type;
};

const std::pair<int, int> getIdentityVector(const char direction)
{
    if (direction == 'U')
        return {-1, 0};
    if (direction == 'D')
        return {1, 0};
    if (direction == 'L')
        return {0, -1};
    // if direction == 'R'
    return {0, 1};
}

VertexType calculateVertexType(const char prev_direction, const char next_direction)
{
    if (prev_direction == 'U')
    {
        if (next_direction == 'L')
            return VertexType::SLASH;
        if (next_direction == 'R')
            return VertexType::BACK_SLASH;
    }
    if (prev_direction == 'L')
    {
        if (next_direction == 'U')
            return VertexType::SLASH;
        if (next_direction == 'D')
            return VertexType::BACK_SLASH;
    }
    if (prev_direction == 'D')
    {
        if (next_direction == 'L')
            return VertexType::BACK_SLASH;
        if (next_direction == 'R')
            return VertexType::SLASH;
    }
    // if prev_direction 'R'
    if (next_direction == 'U')
        return VertexType::BACK_SLASH;
    // if next_direction 'D'
    return VertexType::SLASH;
}

std::pair<int64_t, int64_t> operator*(const int multiplier, const std::pair<int64_t, int64_t> &pair)
{
    return {multiplier * pair.first, multiplier * pair.second};
}

std::pair<int64_t, int64_t> operator+(const std::pair<int64_t, int64_t> &lhs, const std::pair<int64_t, int64_t> &rhs)
{
    return {lhs.first + rhs.first, lhs.second + rhs.second};
}

std::pair<int64_t, int64_t> operator-(const std::pair<int64_t, int64_t> &lhs, const std::pair<int64_t, int64_t> &rhs)
{
    return {lhs.first - rhs.first, lhs.second - rhs.second};
}

int calcArea(const std::pair<int64_t, int64_t> &lhs, const std::pair<int64_t, int64_t> &rhs)
{
    return (lhs.first + rhs.first) * (lhs.second - rhs.second);
}

std::pair<int64_t, char> getInstructionsFromCode(const std::string &code)
{
    std::pair<int64_t, char> instruction;

    instruction.first = std::stoi(code.substr(2, 5), nullptr, 16);

    if (code[7] == '0')
    {
        instruction.second = 'R';
    }
    else if (code[7] == '1')
    {
        instruction.second = 'D';
    }
    else if (code[7] == '2')
    {
        instruction.second = 'L';
    }
    else if (code[7] == '3')
    {
        instruction.second = 'U';
    }

    return instruction;
}

int64_t calculateVolume(const std::vector<InputData> &dig_plan)
{
    std::vector<Vertex> vertexes;
    std::pair<int64_t, int64_t> prev_vertex_pos{0, 0};
    int64_t number_of_blocks_in_wall = 0;
    std::pair<int64_t, char> prev_instruction = getInstructionsFromCode(dig_plan[dig_plan.size() - 1].color_code);
    for (size_t i = 0; i < dig_plan.size(); i++)
    {
        std::pair<int64_t, char> instruction = getInstructionsFromCode(dig_plan[i].color_code);
        
        number_of_blocks_in_wall += instruction.first;
        
        Vertex next_vertex;
        next_vertex.position = prev_vertex_pos + (instruction.first * getIdentityVector(instruction.second));
        next_vertex.type = calculateVertexType(prev_instruction.second, instruction.second);
        vertexes.push_back(next_vertex);
        
        prev_vertex_pos = next_vertex.position;
        prev_instruction = instruction;
    }


    int64_t double_area = 0;
    double edges_1 = 0, edges_2 = 0;
    bool invert = false;
    for (int i = 0, j = vertexes.size() - 1; i < vertexes.size(); j = i, i++)
    {
        double_area += (vertexes[j].position.first + vertexes[i].position.first) * (vertexes[j].position.second - vertexes[i].position.second);

        if (vertexes[j].type == vertexes[i].type)
        {
            invert = !invert;
        }
        edges_1 += invert ? 0.75 : 0.25;
        edges_2 += invert ? 0.25 : 0.75;
    }

    return (int64_t)((std::abs(double_area) + (number_of_blocks_in_wall - vertexes.size())) / 2.0) + (int64_t)std::max(edges_1, edges_2);
}

int main()
{
    std::ifstream file("2023/18/input.txt");
    if (!file.is_open())
    {
        fprintf(stderr, "Unable to open file!\n");
        return 1;
    }
    std::vector<InputData> dig_plan;
    for (std::string line; std::getline(file, line);)
    {
        std::istringstream is(line);
        InputData input_data;
        is >> input_data.direction;
        is >> input_data.length;
        is >> input_data.color_code;
        dig_plan.push_back(input_data);
    }
    file.close();

    printf("%lu\n", calculateVolume(dig_plan));

    return 0;
}