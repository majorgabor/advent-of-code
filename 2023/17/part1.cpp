#include <fstream>
#include <vector>
#include <queue>
#include <limits>
#include <map>

enum class Direction
{
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
};

typedef std::pair<int, int> Position;
typedef std::pair<Position, std::pair<Direction, int>> Step;

const std::vector<Step> moves{
    {{-1, 0}, {Direction::UP, 0}}, {{0, -1}, {Direction::LEFT, 0}}, {{1, 0}, {Direction::DOWN, 0}}, {{0, 1}, {Direction::RIGHT, 0}}};

Step operator+(const Step &lhs, const Step &rhs)
{
    return {{lhs.first.first + rhs.first.first, lhs.first.second + rhs.first.second}, {rhs.second.first, lhs.second.first == rhs.second.first ? lhs.second.second + 1 : 1}};
}

uint modified_dijkstra(const std::vector<std::vector<int>> &mat, const Position &source)
{
    const Step source_step = {source, {Direction::RIGHT, 0}};
    
    std::priority_queue<std::pair<int, Step>, std::vector<std::pair<int, Step>>, std::greater<std::pair<int, Step>>> priority_queue;
    priority_queue.push({0, source_step});

    std::map<Step, int> cumulative_lava_loss;
    cumulative_lava_loss[source_step] = 0;

    while (!priority_queue.empty())
    {
        Step current_step = priority_queue.top().second;
        priority_queue.pop();

        for (const auto &move : moves)
        {
            const Step next_step = current_step + move;
            if ((0 <= next_step.first.first && next_step.first.first < mat.size() && 0 <= next_step.first.second && next_step.first.second < mat[0].size()))
            {
                if (current_step.second.first != (Direction)(((int)next_step.second.first + 2) % 4) && next_step.second.second <= 3)
                {
                    int cumulative_lava_loss_in_next_step = std::numeric_limits<int>::max();
                    if (cumulative_lava_loss.find(next_step) != cumulative_lava_loss.end())
                    {
                        cumulative_lava_loss_in_next_step = cumulative_lava_loss[next_step];
                    }
                    if (mat[next_step.first.first][next_step.first.second] + cumulative_lava_loss[current_step] < cumulative_lava_loss_in_next_step)
                    {
                        cumulative_lava_loss[next_step] = mat[next_step.first.first][next_step.first.second] + cumulative_lava_loss[current_step];
                        priority_queue.push({cumulative_lava_loss[next_step], next_step});
                    }
                }
            }
        }
    }

    int min_laval_loss_at_the_end = std::numeric_limits<int>::max();
    Step final_min_lava_loss_step;
    for (const auto direction : {Direction::DOWN, Direction::RIGHT})
    {
        for (int i = 1; i <= 3; i++)
        {
            if(cumulative_lava_loss[{{mat.size() - 1, mat[0].size() - 1}, {direction, i}}] < min_laval_loss_at_the_end)
            {
                min_laval_loss_at_the_end = cumulative_lava_loss[{{mat.size() - 1, mat[0].size() - 1}, {direction, i}}];
                final_min_lava_loss_step = {{mat.size() - 1, mat[0].size() - 1}, {direction, i}};
            }
        }
    }

    return min_laval_loss_at_the_end;
}

int main()
{
    std::ifstream file("2023/17/input.txt");
    if (!file.is_open())
    {
        fprintf(stderr, "Unable to open file");
        return 1;
    }
    std::vector<std::vector<int>> mat;
    for (std::string line; std::getline(file, line);)
    {
        std::vector<int> temp;
        for (char n : line)
        {
            temp.push_back(n - '0');
        }
        mat.push_back(temp);
    }
    file.close();

    printf("%d\n", modified_dijkstra(mat, {0, 0}));

    return 0;
}
