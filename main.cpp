#include <bits/stdc++.h>
using namespace std;

map<string,string> lookup;

#define max_key_len ((size_t)1024)

string convert(const string& s)
{
    if(s.size() == 1)
        return s;

    if(s.size() <= max_key_len)
        if(lookup.find(s) != lookup.end())
            return s[0] + lookup[s] + s[s.size() - 1];

    const string& str1 {s.substr(0, s.size() >> 1)};
    const string& str2 {s.substr(str1.size(), s.size() - str1.size())};
    const string& str_m {s.substr(str1.size() - 1, 2)};

    string out = convert(str1) + lookup[str_m] + convert(str2);

    if(s.size() <= max_key_len)
        lookup.insert({s, out.substr(1, out.size() - 2)});

    return out;
}

size_t calc(string s, size_t cnt)
{
    for(size_t i = 0; i < cnt; i++)
        s = convert(s);

    map<char,size_t> char_counts;
    for(auto c : s)
        char_counts[c]++;

    vector<size_t> tallies;
    for(auto cc: char_counts)
        tallies.push_back(cc.second);
    sort(tallies.begin(), tallies.end());
    
    return tallies[tallies.size()-1] - tallies[0]; //max - min
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
            lookup.insert({key, val});
        }

        for(auto lu : lookup)
            cout << lu.first << " : " << lu.second << endl;
        cout << endl;
        cout << "input : " << input_s << endl;
        cout << endl;


        start = chrono::high_resolution_clock::now();
        size_t answer = calc(input_s, 10);
        auto stop = chrono::high_resolution_clock::now();
        cout << "a - answer 10: " << answer << endl;
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "sec: " << (duration.count() / 1000000) << 
                        "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
        cout << endl;


        start = chrono::high_resolution_clock::now();
        answer = calc(input_s, 20);
        stop = chrono::high_resolution_clock::now();
        cout << "    answer 20: " << answer << endl;
        duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "sec: " << (duration.count() / 1000000) << 
                        "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
        cout << endl;


        cout << endl;

        infile.close();
    }

    // execution time
    // auto stop = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "sec: " << (duration.count() / 1000000) << 
    //                 "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    // cout << endl;
}
