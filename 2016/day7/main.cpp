#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

// "An ABBA is any four-character sequence which
// consists of a pair of two different characters
// followed by the reverse of that pair,
// such as xyyx or abba"
bool is_abba(const string & s)
{
    size_t pos_last = s.size() - 4;
    for(size_t pos = 0; pos <= pos_last; ++pos)
    {
        if( (s[pos]     == s[pos + 3]) &&
            (s[pos + 1] == s[pos + 2]) &&
            (s[pos]     != s[pos + 1]) )
        {
            return true;
        }
    }
    return false;
}

struct Ip_address{
    string address;
    bool is_ip_valid()
    {
        vector<string> standard;
        vector<string> hypernet;

        string temp;
        for(auto c : address)
        {
            if(c == '[')
            {
                standard.push_back(temp);
                temp.clear();
                continue;
            }
            if(c == ']')
            {
                hypernet.push_back(temp);
                temp.clear();
                continue;
            }
            temp += c;
        }
        standard.push_back(temp);

        if( any_of(hypernet.begin(), hypernet.end(), is_abba) )
            return false;
        if( any_of(standard.begin(), standard.end(), is_abba) )
            return true;
        return false;
    }
};

using Ip_addresses = vector<Ip_address>;


uint32_t ip_valid_count(Ip_addresses ip_addresses)
{
    Timer t{"ip_valid_count"};

    uint32_t tally = 0;

    for(auto ip : ip_addresses)
        if(ip.is_ip_valid())
            tally++;

    return tally;
}

int main()
{
    Timer t_main("main");

    Ip_addresses ip_addresses;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            ip_addresses.push_back({line});
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto result = ip_valid_count(ip_addresses);
    cout << "part 1 - valid ip count: " << result << "\n";
    cout << endl;

}
