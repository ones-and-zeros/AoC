#include <bits/stdc++.h>
#include <cctype>
using namespace std;


int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        string input_s;

        map<string,char> lookup;

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


        for(auto lu : lookup)
            cout << lu.first << " : " << lu.second << endl;
        cout << endl;
        cout << "input : " << input_s << endl;

        string out_s = input_s;
        for(size_t i = 0; i < 10; i++)
        {
            string to_add;
            for(size_t j = 0; j < out_s.size() - 1; j+=1)
                to_add += lookup[out_s.substr(j,2)];

            for(size_t j = 0; j < to_add.size(); j++)
                out_s.insert( 1+(j*2), 1, to_add[j]);

//            cout << "step " << i+1 << ": " << out_s << endl;
        }
        cout << endl;

        map<char,size_t> char_counts;
        for(auto c : out_s)
            char_counts[c]++;

        cout << "char counts:" << endl;
        for(auto cc: char_counts)
            cout << cc.first << " : " << cc.second << endl;
        cout << endl;

        vector<size_t> tallies;
        for(auto cc: char_counts)
            tallies.push_back(cc.second);
        sort(tallies.begin(), tallies.end());
        
        size_t min = tallies[0];
        size_t max = tallies[tallies.size()-1];
        cout << "min: " << min << "  max: " << max << endl;

        size_t answer = max - min;
        cout << "answer: " << answer << endl;
        cout << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
