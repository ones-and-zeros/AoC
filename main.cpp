#include <bits/stdc++.h>
using namespace std;


int32_t goal;
vector<int32_t> containers;

vector<size_t> used;
int32_t sum;

vector<vector<size_t>> exact_combos;

size_t qty_min_count()
{
    size_t min_count = ~0UL;
    for(auto c : exact_combos)
        if(c.size() < min_count)
            min_count = c.size(); 

    size_t qty = 0;
    for(auto c : exact_combos)
        if(c.size() == min_count)
            qty++;
    return qty;
}

void add_container(size_t container)
{
    if(sum > goal)
        return;
    if(sum == goal)
    {
        exact_combos.push_back(used);
        return;
    }

    for(size_t c = container + 1; c < containers.size(); c++)
    {
        sum += containers[c];
        used.push_back(c);
        add_container(c);
        used.pop_back();
        sum -= containers[c];
    }
}

void find_combos(uint32_t total)
{
    goal = total;
    used.clear();
    exact_combos.clear();
    sum = 0;

    for(size_t c = 0; c < containers.size(); c++)
    {
        sum = containers[c];
        used.push_back(c);
        add_container(c);
        used.pop_back();
    }
}


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
            containers.push_back({stoi(line)});
        }

        find_combos(25);
        cout << "ex-a) total 25: " << exact_combos.size() << "\n";
        cout << "ex-b)   min 25: " << qty_min_count() << "\n";
        cout << "\n";

        find_combos(150);
        cout << "a) total 150: " << exact_combos.size() << "\n";
        cout << "b)   min 150: " << qty_min_count() << "\n";

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
