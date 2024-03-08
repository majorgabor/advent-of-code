#include <fstream>
#include <vector>
#include <map>
#include <functional>

struct MachinePart
{
    int x, m, a, s;
    MachinePart(int x, int m, int a, int s) : x(x), m(m), a(a), s(s){};
};

typedef std::function<std::string(const MachinePart &machinePart)> Workflow;

struct Condition
{
    const int threshold;
    const char category;
    std::string result;
    std::function<bool(const MachinePart &machine_part)> condition_func;

    Condition(const std::string str_condition) : threshold(std::stoi(str_condition.substr(2))), category(str_condition[0]), result(str_condition.substr(str_condition.find(':') + 1))
    {
        if (str_condition[1] == '<')
        {
            condition_func = [&](const MachinePart &machine_part)
            {
                if (category == 'x')
                    return machine_part.x < threshold;
                if (category == 'm')
                    return machine_part.m < threshold;
                if (category == 'a')
                    return machine_part.a < threshold;
                // if (category == 's')
                return machine_part.s < threshold;
            };
        }
        else
        {
            condition_func = [&](const MachinePart &machine_part)
            {
                if (category == 'x')
                    return machine_part.x > threshold;
                if (category == 'm')
                    return machine_part.m > threshold;
                if (category == 'a')
                    return machine_part.a > threshold;
                // if (category == 's')
                return machine_part.s > threshold;
            };
        }
    }

    bool checkFor(const MachinePart &machine_part)
    {
        return condition_func(machine_part);
    }
};

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

MachinePart machinePartParser(const std::string &str_part)
{
    auto categories = myStringSplitter(str_part.substr(1, str_part.size() - 2), ',');
    return MachinePart(std::stoi(categories[0].substr(2)), std::stoi(categories[1].substr(2)), std::stoi(categories[2].substr(2)), std::stoi(categories[3].substr(2)));
}

bool isCondition(const std::string condition)
{
    for (char c : condition)
    {
        if (c == '<' || c == '>')
        {
            return true;
        }
    }
    return false;
}

std::string workflow(const std::string &commands, const MachinePart &machinePart)
{
    auto command_list = myStringSplitter(commands, ',');
    for (auto &command : command_list)
    {
        if (isCondition(command))
        {
            Condition condition(command);
            if (condition.checkFor(machinePart))
            {
                return condition.result;
            }
        }
        else
        {
            return command;
        }
    }
}

int main()
{
    std::ifstream file("2023/19/input.txt");
    if (!file.is_open())
    {
        printf("Unable to open file!\n");
        return 1;
    }
    std::map<std::string, Workflow> workflows;
    for (std::string line; std::getline(file, line) && line != "";)
    {
        auto name_and_commands = myStringSplitter(line, '{');
        name_and_commands[1].pop_back();
        workflows.emplace(name_and_commands[0], std::bind(&workflow, name_and_commands[1], std::placeholders::_1));
    }
    int64_t total = 0;
    for (std::string line; std::getline(file, line);)
    {
        MachinePart machine_part = machinePartParser(line);

        std::string current_workflow{"in"};
        for (; current_workflow != "A" && current_workflow != "R"; current_workflow = workflows[current_workflow](machine_part)){};
        if (current_workflow == "A")
        {
            total += machine_part.x + machine_part.m + machine_part.a + machine_part.s;
        }
    }
    printf("%lu\n", total);

    return 0;
}