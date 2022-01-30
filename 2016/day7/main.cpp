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
public:
    Ip_address(string address)
    : addr{address}
    {
        string temp;
        for(auto c : address)
        {
            if(c == '[')
            {
                if(temp.size())
                    supernets.push_back(temp);
                temp.clear();
                continue;
            }
            if(c == ']')
            {
                if(temp.size())
                    hypernets.push_back(temp);
                temp.clear();
                continue;
            }
            temp += c;
        }
        if(temp.size())
            supernets.push_back(temp);
    }

    bool supports_tls()
    {
        if( any_of(hypernets.begin(), hypernets.end(), is_abba) )
            return false;
        if( any_of(supernets.begin(), supernets.end(), is_abba) )
            return true;
        return false;
    }
private:
    string addr;
    vector<string> supernets;
    vector<string> hypernets;
};

using Ip_addresses = vector<Ip_address>;


uint32_t ip_supports_tls_count(Ip_addresses ip_addresses)
{
    Timer t{"ip_supports_tls_count"};

    uint32_t tally = 0;

    for(auto ip : ip_addresses)
        if(ip.supports_tls())
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

    auto result = ip_supports_tls_count(ip_addresses);
    cout << "part 1 - TLS support count: " << result << "\n";
    cout << endl;

}
