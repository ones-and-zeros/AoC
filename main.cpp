#include <bits/stdc++.h>
using namespace std;

multimap<string,string> replacements;

uint64_t steps_min = ~0LL;
uint64_t result_count = 0;
constexpr uint64_t max_results{10000};

void next_step(string in, size_t step)
{
    if(result_count >= max_results)
        return;

    if(in == "e")
    {
        result_count++;
        if(step < steps_min)
            steps_min = step;
        return;
    }

    vector<pair<size_t, string>> all_compounds;
    for(auto r : replacements)
    {
        size_t pos = in.find(r.second, 0);
        while(in.size() > pos)
        {
            string compound;
            compound += in.substr(0, pos);
            compound += r.first;
            compound += in.substr((pos + r.second.size()), (in.size() - (pos + r.second.size())));
            all_compounds.push_back({compound.size(), compound});
            pos = in.find(r.second, pos+1);
        }
    }
    sort(all_compounds.begin(), all_compounds.end());
    for(auto c : all_compounds)
        next_step(c.second, step + 1);
}

uint64_t steps_to_create(string target)
{
    steps_min = ~0LL;
    result_count = 0;

    next_step(target, 0);

    return steps_min;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        string original;

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
        

        uint64_t step_qty = steps_to_create(original);
        cout << "b) steps to create: " << step_qty << "\n";


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
