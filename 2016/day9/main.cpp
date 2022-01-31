#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

uint64_t decomp_help(const string & in, bool do_full = false)
{
    uint64_t length = 0;
    istringstream iss{in};
    char c;
    while(iss >> c)
    {
        if(c == ')')
            throw logic_error("invaid terminator");

        if(c != '(')
        {
            length++;
            continue;
        }

        // '(' found so extract
        int32_t size, qty;
        iss >> size;
        iss >> c; // junk 'x'
        iss >> qty;
        iss >> c; // junk ')'

        string substring;
        for(size_t n = 0; n < size; n++)
        {
            iss >> c;
            substring += c;
        }

        if(do_full)
            length += decomp_help(substring, true) * qty;
        else
            length += size * qty;
    }

    return length;
}

uint64_t decompressed_length(const string & in, bool do_full = false)
{
    Timer t{"decompressed_length"};
    return decomp_help(in, do_full);
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

    uint64_t result = decompressed_length(input);
    cout << "part 1 - decompressed length: " << result << "\n";
    cout << endl;

    result = decompressed_length(input, true);
    cout << "part 2 - decompressed length full: " << result << "\n";
    cout << endl;
}
