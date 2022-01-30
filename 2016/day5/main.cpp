#include <bits/stdc++.h>
using namespace std;

#include "timer.h"
#include "md5.h"

constexpr size_t password_length{8};

string password(string door_id)
{
    Timer t{"password"};

    string result;

    uint64_t index_val = 0;
    while(1)
    {
        auto hash_val = md5(door_id + to_string(index_val));

        if("00000" == hash_val.substr(0,5))
        {
            result += hash_val[5];
            if(result.size() == password_length)
                break;
        }
        index_val++;
    }

    return result;
}

string better_password(string door_id)
{
    Timer t{"better password"};

    string result(password_length, ' ');

    uint64_t found = 0;
    for(uint64_t index_val = 0; found != 0b11111111; index_val++)
    {
        auto hash_val = md5(door_id + to_string(index_val));

        if("00000" == hash_val.substr(0,5))
        {
            size_t pos = hash_val[5] - '0';
            if(pos >= password_length)
                continue;

            if(1 == ((found >> pos) & 1))
                continue;
            found |= (1UL << pos);

            result[pos] = hash_val[6];
        }
    }

    return result;
}

int main()
{
    Timer t_main("main");

    string input;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            input = line;
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto result = password(input);
    cout << "part 1 - password: " << result << endl;
    cout << "\n";

    auto result2 = better_password(input);
    cout << "part 2 - better: " << result2 << endl;
    cout << "\n";
}
