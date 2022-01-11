#include <bits/stdc++.h>
#include <cctype>
using namespace std;


using path = pair<string,string>;
using cave = string;

set<path> paths;
set<cave> caves;
vector<cave> visited;
vector<vector<cave>> complete;

bool small_cave_twice = false;

void visit(const cave& entered)
{
    bool is_sec_small_cave = false;
    if(islower(entered[0]))
    {
        if(find(visited.begin(), visited.end(), entered) != visited.end())
        {
            if(small_cave_twice || entered == "start")
                return;
            is_sec_small_cave = true;
            small_cave_twice = true;
        }
    }

    //enter cave
    visited.push_back(entered);

    // check for final destination
    if(entered == "end")
    {
        complete.push_back(visited); // tally this path as complete
        visited.erase(visited.end() - 1); // return to previous cave
        return;
    }

    //not final destination, so pick next cave
    for(auto p : paths)
    {
        if(p.first == entered)
            visit(p.second);
        else if(p.second == entered)
            visit(p.first);
    }


    if(is_sec_small_cave)
        small_cave_twice = false;
    visited.erase(visited.end()-1);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        //parse lines
        while(getline(infile, file_line))
        {
            istringstream iss{file_line};
            string s1, s2;
            getline(iss, s1, '-');
            getline(iss, s2);
            paths.insert({s1,s2});
            caves.insert(s1);
            caves.insert(s2);
        }

        visit("start");

        // cout << "caves:" << endl;
        // for(auto c : caves)
        //     cout << " " << c << endl;
        // cout << endl;


        // cout << "paths:" << endl;
        // for(auto p : paths)
        //     cout << " " << p.first << "-" << p.second << endl;
        // cout << endl;

        size_t cnt = 0;
        for(auto c : complete)
        {
            cnt++;
            cout << cnt << ": ";
            for(auto v : c)
                cout << v << " ";
            cout << endl;
        }

        cout << " b - total paths: " << complete.size() << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
