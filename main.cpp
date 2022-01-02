#include <bits/stdc++.h>
using namespace std;

using Tally = map<string,uint64_t>;

map<string,char> lookup;

Tally tally_input(string in)
{
    Tally tally;

    // for(auto l : lookup)
    //     tally.insert({l.first, 0});

    for(size_t i = 0; i < in.size() - 1; i++)
    {
        string substr = in.substr(i,2);
        tally[substr]++;
    }
    return tally;
}

void print_tally(Tally tally)
{
    for(auto t : tally)
        cout << t.first << ": " << t.second << '\n';
}

void step_tally(Tally& tally, string& trail)
{
    Tally next_tally;

    for(auto t : tally)
    {
        string sub1 = string(1, t.first[0]) + string(1, lookup[t.first]);
        string sub2 = string(1, lookup[t.first]) + string(1, t.first[1]);
        next_tally[sub1] += t.second;
        next_tally[sub2] += t.second;
    }

    // will be used for accurate final count of chars
    trail = string(1, lookup[trail]) + string(1, trail[1]);

    tally = next_tally;
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        string input_s;

        //parse lines
        while(getline(infile, file_line))
        {
            if(file_line.empty())
                continue;

            if(input_s.empty())
            {
                input_s = file_line;
                continue;
            }

            istringstream iss{file_line};
            string key, val, junk;
            getline(iss, key, ' ');
            getline(iss, junk, ' ');
            getline(iss, val, ' ');
            lookup.insert({key, val[0]});
        }

        start = chrono::high_resolution_clock::now();

        map<string,uint64_t> tally = {tally_input(input_s)};
        string trailing_char_pair = input_s.substr(input_s.size()-2, 2);

        cout << "input: " << input_s << '\n';
//        print_tally(tally);
        cout << '\n';


        // first 10 steps - part a
        for(size_t i = 0; i < 10; i++)
        {
            step_tally(tally, trailing_char_pair);
            // cout << "step" << i+1 << ":\n";
            // print_tally(tally);
            // cout << '\n';
        }

        map<char, uint64_t> char_tally;
        for(auto t : tally)
            char_tally[t.first[0]] += t.second;
        char_tally[trailing_char_pair[1]]++;

        for(auto ct : char_tally)
            cout << ct.first << ": " << ct.second << "\n";

        vector<uint64_t> char_totals;
        for(auto t : char_tally)
            char_totals.push_back(t.second);
        sort(char_totals.begin(), char_totals.end());

        cout << "a 10x: max - min = " <<
                char_totals[char_totals.size()-1] << " - " << char_totals[0] <<
                " = " << char_totals[char_totals.size()-1] - char_totals[0] << "\n";
        cout << "\n";

        // last 30, tota l 40
        for(size_t i = 0; i < 30; i++)
            step_tally(tally, trailing_char_pair);

        char_tally.clear();
        for(auto t : tally)
            char_tally[t.first[0]] += t.second;
        char_tally[trailing_char_pair[1]]++;

        // for(auto ct : char_tally)
        //     cout << ct.first << ": " << ct.second << "\n";

        char_totals.clear();
        for(auto t : char_tally)
            char_totals.push_back(t.second);
        sort(char_totals.begin(), char_totals.end());

        cout << "b 40x: max - min = " <<
                char_totals[char_totals.size()-1] << " - " << char_totals[0] <<
                " = " << char_totals[char_totals.size()-1] - char_totals[0] << "\n";


        cout << endl;
        infile.close();
    }

    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
