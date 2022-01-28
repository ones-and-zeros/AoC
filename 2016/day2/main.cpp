#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

class Keypad{
public:
    Keypad()
    {
    }
    char update_pos(string commands)
    {
        for(auto c : commands)
        {
            int32_t &axis = ((c == 'U') || (c =='D')) ? x : y;
            axis += ((c == 'D') || (c =='R')) ? keypad_step : -keypad_step;

            if(axis < 0)
            {
                axis = 0;
            }
            else if(axis >= keypad_axis_len)
            {
                axis = keypad_axis_len - 1;
            }
        }

        return keys[x][y];
    }
private:
    int32_t x = 1; //point to '5'
    int32_t y = 1; //point to '5'
    // consts
    static constexpr int32_t keypad_axis_len{3};
    static constexpr int32_t keypad_step{1};
    static constexpr array<array<char,keypad_axis_len>, keypad_axis_len> keys{
        array<char,keypad_axis_len>{'1','2','3'},
        array<char,keypad_axis_len>{'4','5','6'},
        array<char,keypad_axis_len>{'7','8','9'} };
};

// needs this declaration outside of class to avoid link error when accessing within class
constexpr array<array<char,Keypad::keypad_axis_len>, Keypad::keypad_axis_len> Keypad::keys;

string find_code(vector<string> commands)
{
    Timer t("find-code");
    Keypad keypad;
    string code;

    for(auto command : commands)
    {
        code += keypad.update_pos(command);
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
