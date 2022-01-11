#include <bits/stdc++.h>
using namespace std;

map<pair<int,int>,long> lookup;

long calc(int val, int days)
{
    pair<int,int> p({val,days});
    auto it = lookup.find(p);
    if(it != lookup.end())
        return lookup[p];

    long result = 1;
    if(days)
    {
        while(days > val)
        {
            days -= (val + 1);
            val = 6;
            result += calc(8, days);
        }
    }
    lookup.insert({p, result});
    return result;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;   

        vector<int> fish_timers;

        //parse lines
        while(getline(infile, file_line))
        {
            istringstream iss(move(file_line));
            string s;
            while(getline(iss, s, ','))
                fish_timers.push_back(stoi(s));
        }

        long long sum = 0;
        for(auto f : fish_timers)
            sum += calc(f, 256);
        cout << "unique: " << lookup.size() << endl;
        cout << " total: " << sum << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
