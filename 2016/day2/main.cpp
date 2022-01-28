#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

vector<string> keypad{
    {"123"},
    {"456"},
    {"789"}
};

constexpr int32_t keypad_axis_len{3};
constexpr int32_t keypad_step{1};

class Keypad_place{
public:
    Keypad_place()
    {
    }
    char update_pos(string commands)
    {
        for(auto c : commands)
        {
            int32_t &axis = ((c == 'U') || (c =='D')) ? x : y;

            if((c == 'D') || (c =='R'))
            {
                if(axis + keypad_step < keypad_axis_len)
                {
                    axis += keypad_step;
                }
            }
            else
            {
                if(axis - keypad_step >= 0)
                {
                    axis -= keypad_step;
                }
            }
        }
        return keypad[x][y];
    }
private:
    int32_t x = 1;
    int32_t y = 1;
};

string find_code(vector<string> commands)
{
    Timer t("find-code");
    Keypad_place place;
    string code;

    for(auto command : commands)
    {
        code += place.update_pos(command);
    }

    return code;
}

int main()
{
    Timer t_main("main");

    vector<string> commands;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            commands.push_back(line);
        }
        infile.close();
    }
    else
        throw logic_error("unable to open input file");

    auto code = find_code(commands);
    cout << "part 1 - code: " << code << "\n";
}
