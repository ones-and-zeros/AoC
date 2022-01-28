#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

struct Command{
    Command(char turn, int32_t length)
    : turn_{turn}, len_{length}
    {
    }
    char turn_;
    int32_t len_;
};

using Commands = vector<Command>;

ostream &operator<<(ostream &os, const Command &command)
{
    os << command.turn_ << "-" << command.len_;
    return os;
}

int32_t short_path_grid(Commands commands)
{
    Timer t("path calc");

    uint32_t direction = 0;
    int32_t x = 0;
    int32_t y = 0;

    for(auto c : commands)
    {
        if(c.turn_ == 'R')  direction += 1;
        if(c.turn_ == 'L')  direction += 3;
        direction &= 0x03;

        auto &axis = (direction & 1) ? x : y;
        int32_t polarity = (direction & 2) ? 1 : -1;

        axis += polarity * c.len_;
    }

    return abs(x) + abs(y);
}

int32_t short_path_first_double(Commands commands)
{
    Timer t("path first double calc");

    uint32_t direction = 0;
    int32_t x = 0;
    int32_t y = 0;
    set<pair<int32_t, int32_t>> visited;
    visited.insert({0,0});

    for(auto c : commands)
    {
        if(c.turn_ == 'R')  direction += 1;
        if(c.turn_ == 'L')  direction += 3;
        direction &= 0x03;

        auto &axis = (direction & 1) ? x : y;
        int32_t polarity = (direction & 2) ? 1 : -1;

        while(c.len_--)
        {
            axis += polarity;
            if(visited.end() != visited.find({x,y}))
                return abs(x) + abs(y);
            visited.insert({x,y});
        }
    }

    return -1;
}

int main()
{
    Timer t_main("main");

    Commands commands;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_in("parse in");

        string line;
        while(getline(infile, line))
        {
            istringstream iss{line};

            string command;
            while(getline(iss, command, ','))
            {
                istringstream iss{command};

                char dir;
                iss >> dir;
                if(dir != 'L' && dir != 'R')
                    throw logic_error("parse invalid direction");
                int32_t len;
                iss >> len;

                commands.push_back({dir, len});
            }
        }
        infile.close();
    }
    else
        throw logic_error("unable to open input file");

    auto result = short_path_grid(commands);
    cout << "part1 - shortest path: " << result << "\n";
    cout << "\n";
    result = short_path_first_double(commands);
    cout << "part2 - shortest path first double: " << result << "\n";
    cout << "\n";
}
