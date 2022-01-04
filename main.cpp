#include <bits/stdc++.h>
using namespace std;


string convert(string original)
{
    string out;

    char letter_last = original[0];
    size_t tally = 1;
    for(size_t i = 1; i < original.size(); i++)
    {
        if(original[i] == letter_last)
        {
            tally++;
            continue;
        }
        out += to_string(tally);
        out += letter_last;
        letter_last = original[i];
        tally = 1;
    }
    out += to_string(tally);
    out += letter_last;

    return out;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> s_in;

        //parse lines
        while(getline(infile, line))
        {
            s_in.push_back(line);
        }

        for(auto s : s_in)
        {
            cout << s << ": " << "\n";
            for(size_t i = 0; i < 40; i++)
            {
                s = convert(s);
//                cout << "  step " << i + 1 << ": " << s << "\n";
            }
            cout << "a) 40x final size: " << s.size() << "\n";
            for(size_t i = 0; i < 10; i++)
            {
                s = convert(s);
//                cout << "  step " << i + 1 << ": " << s << "\n";
            }
            cout << "b) 50x final size: " << s.size() << "\n";
        }



        cout << '\n';
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
