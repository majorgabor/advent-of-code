#include <fstream>
#include <vector>

uint hash(const std::string &step)
{
    uint value = 0u;
    for (char c : step)
    {
        value = (((value + int(c)) * 17) % 256);
    }
    return value;
}

int main()
{
    std::fstream file("2023/15/input.txt", std::ios_base::in);
    if (file.is_open())
    {
        uint result = 0u;
        std::string initialization_step;
        while (getline(file, initialization_step, ','))
        {
            result += hash(initialization_step);
        }
        printf("%u\n", result);
    }
    else
    {
        printf("Unable to open file!");
    }

    return 0;
}