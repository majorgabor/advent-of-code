#include <vector>
#include <string>
#include <map>
#include <algorithm>
#include <fstream>
#include <numeric>

int64_t gcd(int64_t a, int64_t b)
{
    for (;;)
    {
        if (a == 0) return b;
        b %= a;
        if (b == 0) return a;
        a %= b;
    }
}

int64_t lcm(int64_t a, int64_t b)
{
    int64_t temp = gcd(a, b);

    return temp ? (a / temp * b) : 0;
}

// HIGH true
// LOW  false

class Module;

struct Pulse
{
    std::string source_module;
    bool pulse_value;
    std::vector<std::string> destinations;
};

std::map<std::string, Module *> modules;

class Module
{
protected:
    const std::string own_name;
    std::vector<std::string> destination_modules;

    Module(const std::string &name, const std::vector<std::string> &destination_modules) : own_name(name), destination_modules(destination_modules) {}

public:
    virtual Pulse operator()(const std::string &source, const bool incoming_pulse) = 0;

    ~Module() {}

public:
    std::vector<std::string> getDestinations() const
    {
        return destination_modules;
    }
};

class FlipFlop : public Module
{
    bool inner_state{false};

public:
    Pulse operator()(const std::string &source, const bool incoming_pulse) override
    {
        if (!incoming_pulse) // if LOW
        {
            inner_state = !inner_state; // flip ON to OFF / OFF to ON
            return {own_name, inner_state, destination_modules};
        }
        return {};
    }

    FlipFlop(const std::string &name, const std::vector<std::string> &destination_modules) : Module(name, destination_modules) {}
    ~FlipFlop() {}
};

class Conjunction : public Module
{
    std::map<std::string, bool> input_pulses;

    inline bool areAllInputPulseHIGH()
    {
        return input_pulses.end() == std::find_if(input_pulses.begin(), input_pulses.end(), [](const auto &input)
                                                  { return !input.second; }); // search for LOW
    }

public:
    Pulse operator()(const std::string &source, const bool incoming_pulse) override
    {
        input_pulses[source] = incoming_pulse;
        return {own_name, !areAllInputPulseHIGH(), destination_modules};
    }

    void addInput(const std::string &module_id)
    {
        input_pulses.insert({module_id, false});
    }

    std::vector<std::string> getInputs()
    {
        std::vector<std::string> keys;
        for (const auto &input : input_pulses)
        {
            keys.push_back(input.first);
        }
        return keys;
    }

    Conjunction(const std::string &name, const std::vector<std::string> &destination_modules) : Module(name, destination_modules) {}
    ~Conjunction() {}
};

class Broadcaster : public Module
{
public:
    Pulse operator()(const std::string &source, const bool incoming_pulse) override
    {
        return {own_name, incoming_pulse, destination_modules};
    }

    Broadcaster(const std::vector<std::string> &destination_modules) : Module("broadcaster", destination_modules) {}
    ~Broadcaster() {}
};

std::vector<std::string> myStringSplitter(const std::string &string_to_split, const char delimeter)
{
    std::vector<std::string> splits;
    for (size_t i{0u}, start_pos{0u}; i <= string_to_split.size(); i++)
    {
        if (string_to_split[i] == delimeter || i >= string_to_split.size())
        {
            splits.push_back(string_to_split.substr(start_pos, i - start_pos));
            start_pos = i + 2u;
        }
    }
    return splits;
}

int main()
{
    std::ifstream file("2023/20/input.txt");
    if (!file.is_open())
    {
        printf("Unable to open file!");
        return 1;
    }
    std::vector<std::string> flip_flop_ids, conjuction_ids;
    for (std::string line; std::getline(file, line);)
    {
        if (line[0] == '%')
        {
            const std::string module_name{line.substr(1, 2)};
            flip_flop_ids.push_back(module_name);
            modules.insert({module_name, new FlipFlop(module_name, myStringSplitter(line.substr(7), ','))});
        }
        else if (line[0] == '&')
        {
            const std::string module_name{line.substr(1, 2)};
            conjuction_ids.push_back(module_name);
            modules.insert({module_name, new Conjunction(module_name, myStringSplitter(line.substr(7), ','))});
        }
        else if ((line[0] == 'b'))
        {
            modules.insert({"broadcaster", new Broadcaster(myStringSplitter(line.substr(15), ','))});
        }
        else
        {
            printf("Undefined module input");
        }
    }
    // setup conjuction inputs
    std::vector<std::string> module_ids;
    for (const auto &module : modules)
    {
        if (module.first != "broadcaster")
        {
            module_ids.push_back(module.first);
        }
    }
    for (auto conjuction_id : conjuction_ids)
    {
        for (auto &module_id : module_ids)
        {
            if (conjuction_id == module_id)
            {
                continue;
            }
            for (auto &destination : modules[module_id]->getDestinations())
            {
                if (destination == conjuction_id)
                {
                    dynamic_cast<Conjunction *>(modules[conjuction_id])->addInput(module_id);
                    break;
                }
            }
        }
    }

    auto modules_to_monitor = dynamic_cast<Conjunction *>(modules["qb"])->getInputs();
    std::map<std::string, size_t> high_pulse_iteration;
    bool searching = true;
    for (size_t i = 0; searching; i++)
    {
        std::vector<Pulse> current_step{modules["broadcaster"]->operator()("aptly", false)};
        while (!current_step.empty())
        {
            std::vector<Pulse> next_step;
            for (const auto &pulse : current_step)
            {
                if (std::find(modules_to_monitor.begin(), modules_to_monitor.end(), pulse.source_module) != modules_to_monitor.end() && pulse.pulse_value)
                {
                    if (!high_pulse_iteration.count(pulse.source_module))
                    {
                        high_pulse_iteration[pulse.source_module] = i + 1;
                        if (high_pulse_iteration.size() == modules_to_monitor.size())
                        {
                            searching = false;
                        }
                    }
                }
                for (const auto &destination : pulse.destinations)
                {
                    if (modules.count(destination))
                    {
                        next_step.push_back(modules[destination]->operator()(pulse.source_module, pulse.pulse_value));
                    }
                }
            }
            current_step = next_step;
            next_step.clear();
        }
    }
    // find LMC
    std::vector<int64_t> iterations;
    for (const auto &high_pulse : high_pulse_iteration)
    {
        iterations.push_back((int64_t)high_pulse.second);
    }
    printf("%lu\n", std::accumulate(iterations.begin(), iterations.end(), 1lu, lcm));

    // finally
    for (auto module : modules)
    {
        delete module.second;
    }

    return 0;
}