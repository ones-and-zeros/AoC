#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

using Messages = vector<string>;

string recover(Messages messages, bool use_min)
{
    Timer t_main("recover");
    
    if(messages.empty())
        return "(no messages)";

    string result;

    for(size_t pos = 0; pos < messages[0].size(); ++pos)
    {
        array<uint32_t, 26> letter_count;
        letter_count.fill(use_min ? ~0 : 0);
        
        for(auto & m : messages)
            ++letter_count[m[pos] - 'a'];

        if(use_min)
            result += 'a' + (min_element(letter_count.begin(),letter_count.end()) - letter_count.begin());
        else
            result += 'a' + (max_element(letter_count.begin(),letter_count.end()) - letter_count.begin());
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

    auto result = recover(messages, false);
    cout << "part 1 - recovered: " << result << "\n";
    cout << "\n";

    result = recover(messages, true);
    cout << "part 2 - recovered with min: " << result << "\n";
    cout << "\n";
}
