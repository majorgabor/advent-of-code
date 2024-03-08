#include <fstream>
#include <vector>
#include <map>
#include <functional>
#include <memory>

typedef std::map<char, std::pair<int, int>> MachinePartIntervals;
typedef std::function<std::vector<std::pair<std::string, MachinePartIntervals>>(const MachinePartIntervals &machine_part_intervals)> Workflow;
std::map<std::string, Workflow> workflows;

std::vector<std::string> myStringSplitter(const std::string &string_to_split, const char delimeter)
{
    std::vector<std::string> splits;
    for (size_t i{0u}, start_pos{0u}; i <= string_to_split.size(); i++)
    {
        if (string_to_split[i] == delimeter || i == string_to_split.size())
        {
            splits.push_back(string_to_split.substr(start_pos, i - start_pos));
            start_pos = i + 1u;
        }
    }
    return splits;
}

inline bool isCondition(const std::string condition)
{
    return condition.length() > 1 && (condition[1] == '<' || condition[1] == '>');
}

int64_t calcAcceptableCombinations(const std::string &current_workflow, const MachinePartIntervals &machine_part_intervals)
{
    if (current_workflow == "R")
    {
        return 0l;
    }
    if (current_workflow == "A")
    {
        int64_t combinations{1l};
        for (char category : {'x', 'm', 'a', 's'})
        {
            combinations *= machine_part_intervals.at(category).second - machine_part_intervals.at(category).first + 1;
        }
        return combinations;
    }

    int64_t total{0l};
    std::vector<std::pair<std::string, MachinePartIntervals>> next_steps = workflows[current_workflow](machine_part_intervals);
    for (auto &next_step : next_steps)
    {
        total += calcAcceptableCombinations(next_step.first, next_step.second);
    }
    return total;
}

std::vector<std::pair<std::string, MachinePartIntervals>> workflow(const std::string &commands, const MachinePartIntervals &machine_part_intervals)
{
    MachinePartIntervals workflow_machine_part_intervals = machine_part_intervals;
    std::vector<std::pair<std::string, MachinePartIntervals>> diff_outcomes;
    auto command_list = myStringSplitter(commands, ',');
    for (auto &command : command_list)
    {
        if (isCondition(command))
        {
            char category = command[0];
            int split_threshold = std::stoi(command.substr(2));
            std::pair<int, int> interval_to_split = workflow_machine_part_intervals[category];
            if (command[1] == '>')
            {
                if (interval_to_split.first <= split_threshold && split_threshold < interval_to_split.second)
                {
                    MachinePartIntervals splitted_machine_part_intervals = workflow_machine_part_intervals;
                    std::pair<int, int> splitted_interval{split_threshold + 1, interval_to_split.second}, modified_interval{interval_to_split.first, split_threshold};
                    splitted_machine_part_intervals[category] = splitted_interval;
                    workflow_machine_part_intervals[category] = modified_interval;
                    diff_outcomes.push_back({command.substr(command.find(':') + 1), splitted_machine_part_intervals});
                }
                else if (split_threshold < interval_to_split.first)
                {
                    diff_outcomes.push_back({command.substr(command.find(':') + 1), workflow_machine_part_intervals});
                    break;
                }
            }
            else // '<'
            {
                if (interval_to_split.first < split_threshold &&  split_threshold <= interval_to_split.second)
                {
                    MachinePartIntervals splitted_machine_part_intervals = workflow_machine_part_intervals;
                    std::pair<int, int> splitted_interval{interval_to_split.first, split_threshold - 1}, modified_interval{split_threshold, interval_to_split.second};
                    splitted_machine_part_intervals[category] = splitted_interval;
                    workflow_machine_part_intervals[category] = modified_interval;
                    diff_outcomes.push_back({command.substr(command.find(':') + 1), splitted_machine_part_intervals});
                }
                else if (interval_to_split.second < split_threshold)
                {
                    diff_outcomes.push_back({command.substr(command.find(':') + 1), workflow_machine_part_intervals});
                    break;
                }
            }
        }
        else
        {
            diff_outcomes.push_back({command, workflow_machine_part_intervals});
        }
    }
    return diff_outcomes;
}


int main()
{
    std::ifstream file("2023/19/input.txt");
    if (!file.is_open())
    {
        printf("Unable to open file!\n");
        return 1;
    }
    for (std::string line; std::getline(file, line) && line != "";)
    {
        auto name_and_commands = myStringSplitter(line, '{');
        name_and_commands[1].pop_back();

        workflows.emplace(name_and_commands[0], std::bind(&workflow, name_and_commands[1], std::placeholders::_1));
    }
    MachinePartIntervals machine_part_intervals{{'x', {1, 4000}}, {'m', {1, 4000}}, {'a', {1, 4000}}, {'s', {1, 4000}}};
    printf("%ld\n", calcAcceptableCombinations("in", machine_part_intervals));

    return 0;
}