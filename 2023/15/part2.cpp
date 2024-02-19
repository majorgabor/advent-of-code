#include <fstream>
#include <vector>
#include <map>
#include <list>
#include <utility>
#include <algorithm>

typedef std::pair<std::string, uint> Lens;
typedef std::map<uint, std::list<Lens>> Boxes;

uint hash(const std::string &step)
{
    uint value = 0u;
    for (char c : step)
    {
        value = (((value + int(c)) * 17) % 256);
    }
    return value;
}

inline bool isLensInList(Lens &lens, const std::string &label)
{
    return lens.first == label;
}

void removeLens(Boxes &boxes, const std::string &init_step)
{
    const std::string label = init_step.substr(0, init_step.length() - 1);
    if (auto box_it = boxes.find(hash(label)); box_it != boxes.end())
    {
        box_it->second.remove_if(std::bind(isLensInList, std::placeholders::_1, label));
    }
}

void addLens(Boxes &boxes, const std::string &init_step)
{
    const std::string label = init_step.substr(0, init_step.length() - 2);
    const uint focal_length = uint(init_step.back() - '0');

    const uint box_number = hash(label);
    if (boxes.count(box_number))
    {
        auto *list = &(boxes.find(box_number)->second);
        auto lens_it = std::find_if(list->begin(), list->end(), std::bind(isLensInList, std::placeholders::_1, label));
        if (lens_it != list->end())
        {
            lens_it->first = label;
            lens_it->second = focal_length;
        }
        else
        {
            list->push_back(Lens{label, focal_length});
        }
    }
    else
    {
        boxes.insert({box_number, std::list<Lens>{Lens{label, focal_length}}});
    }
}

int main()
{

    std::fstream file("2023/15/input.txt", std::ios_base::in);
    if (file.is_open())
    {
        Boxes boxes;

        uint result = 0u;
        std::string initialization_step;
        while (getline(file, initialization_step, ','))
        {

            if (initialization_step.back() == '-')
            {
                removeLens(boxes, initialization_step);
            }
            else
            {
                addLens(boxes, initialization_step);
            }
        }
        for (auto &box : boxes)
        {
            uint box_number = box.first;
            uint slot = 1u;
            for (auto & lens : box.second)
            {
                result += (box.first + 1) * slot++ * lens.second;
            }
        }
        printf("%u\n", result);
    }
    else
    {
        printf("Unable to open file!");
    }

    return 0;
}