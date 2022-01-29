#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

class Keypad{
public:
    virtual char update_pos(string commands) = 0;
};

class Keypad_basic : public Keypad{
public:
    Keypad_basic()
    {
    }
    char update_pos(string commands) override
    {
        for(auto c : commands)
        {
            int32_t &axis = ((c == 'U') || (c =='D')) ? x : y;
            axis += ((c == 'D') || (c =='R')) ? 1 : -1;

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
    // point to '5'
    int32_t x = 1;
    int32_t y = 1;
    // consts
    static constexpr int32_t keypad_axis_len{3};
    static constexpr array<array<char,keypad_axis_len>, keypad_axis_len> keys{
        array<char,keypad_axis_len>{'1','2','3'},
        array<char,keypad_axis_len>{'4','5','6'},
        array<char,keypad_axis_len>{'7','8','9'} };
};

// declaration outside of class to avoid link error when accessing within class
constexpr array<array<char,Keypad_basic::keypad_axis_len>, Keypad_basic::keypad_axis_len> Keypad_basic::keys;

class Keypad_advanced : public Keypad {
public:
    Keypad_advanced()
    {
    }
    char update_pos(string commands) override
    {
        for(auto c : commands)
        {
            switch(c)
            {
                case 'U':
                {
                    if(x > keypad_axis_center - keypad_axis_deviation[y])
                        --x;
                    break;
                }
                case 'D':
                {
                    if(x < keypad_axis_center + keypad_axis_deviation[y])
                        ++x;
                    break;
                }
                case 'L':
                {
                    if(y > keypad_axis_center - keypad_axis_deviation[x])
                        --y;
                    break;
                }
                case 'R':
                {
                    if(y < keypad_axis_center + keypad_axis_deviation[x])
                        ++y;
                    break;
                }
            }
        }

        return keys[x][y];
    }
private:
    // point to '5'
    int32_t x = 2;
    int32_t y = 0;
    // consts
    static constexpr int32_t keypad_axis_len{5};
    static constexpr int32_t keypad_axis_center{(keypad_axis_len - 1) / 2};
    static constexpr array<int32_t,keypad_axis_len> keypad_axis_deviation{0,1,2,1,0};
    static constexpr array<array<char,keypad_axis_len>, keypad_axis_len> keys{
        array<char,keypad_axis_len>{' ',' ','1',' ',' '},
        array<char,keypad_axis_len>{' ','2','3','4',' '},
        array<char,keypad_axis_len>{'5','6','7','8','9'},
        array<char,keypad_axis_len>{' ','A','B','C',' '},
        array<char,keypad_axis_len>{' ',' ','D',' ',' '} };
};

// declarations outside of class to avoid link error when accessing within class
constexpr array<int32_t,Keypad_advanced::keypad_axis_len> Keypad_advanced::keypad_axis_deviation;
constexpr array<array<char,Keypad_advanced::keypad_axis_len>, Keypad_advanced::keypad_axis_len> Keypad_advanced::keys;

unique_ptr<Keypad> keypad_create(bool advanced)
{
    if(advanced)
        return unique_ptr<Keypad>{new Keypad_advanced{}};
    return unique_ptr<Keypad>{new Keypad_basic{}};
}

string find_code(vector<string> commands, bool advanced)
{
    Timer t("find-code");
    auto keypad = keypad_create(advanced);
    string code;

    for(auto command : commands)
    {
        code += keypad->update_pos(command);
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

    auto code = find_code(commands, false);
    cout << "part 1 - code: " << code << "\n";
    cout << "\n";

    code = find_code(commands, true);
    cout << "part 2 - code: " << code << "\n";
    cout << "\n";
}
