#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

using Messages = vector<string>;

string recover(Messages messages)
{
    Timer t_main("recover");
    
    if(messages.empty())
        return "(no messages)";

    string result;

    for(size_t pos = 0; pos < messages[0].size(); ++pos)
    {
        array<uint32_t, 26> letter_count{0};
        for(auto & m : messages)
            ++letter_count[m[pos] - 'a'];

        uint32_t max_val = 0;
        char letter = '?';
        for(size_t lpos = 0; lpos < 26; ++lpos)
        {
            if(letter_count[lpos] > max_val)
            {
                max_val = letter_count[lpos];
                letter = 'a' + lpos;
            }
        }
        result += letter;
    }

    return result;
}


int main()
{
    Timer t_main("main");

    Messages messages;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            messages.push_back(line);
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto result = recover(messages);
    cout << "part 1 - recovered: " << result << "\n";
    cout << "\n";
}
