#include <bits/stdc++.h>
using namespace std;


multimap<string,string> replacements;
string original;

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        //parse lines
        while(getline(infile, line))
        {
            if(line.empty())
                continue;

            istringstream iss{line};
            string org;
            string arrow;
            iss >> org;
            if(iss >> arrow)
            {
                string replace;
                iss >> replace;
                replacements.insert({org, replace});
            }
            else
                original = org;
        }

        // for(auto r : replacements)
        // {
        //     cout << r.first << " to " << r.second << "\n";
        // }
        // cout << "org: " << original << "\n";


        set<string> unique_compounds;
        for(auto r : replacements)
        {
            size_t pos = original.find(r.first, 0);
            while(original.size() > pos)
            {
                string compound;
                compound += original.substr(0, pos);
                compound += r.second;
                compound += original.substr((pos + r.first.size()), (original.size() - (pos + r.first.size())));
                unique_compounds.insert(compound);

                pos = original.find(r.first, pos+1);
            }
        }

        // cout << "unique:\n";
        // for(auto s : unique_compounds)
        //     cout << s << "\n";

        cout << "a) unique qty: " << unique_compounds.size() << "\n";
        


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
