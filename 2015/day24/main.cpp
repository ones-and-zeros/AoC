#include <bits/stdc++.h>
using namespace std;

vector<uint32_t> all_pkgs;
vector<uint32_t> pkg_bunch;
vector<vector<uint32_t>> pkg_bunches;
uint32_t current_weight = 0;
uint32_t target_weight = 0;
uint32_t min_package_qty = 0;

void select_pkg(size_t offset)
{
    if(current_weight == target_weight)
    {
        if(pkg_bunch.size() < min_package_qty)
        {
            min_package_qty = pkg_bunch.size();
            pkg_bunches.clear();
        }
        pkg_bunches.push_back(pkg_bunch);
        return;
    }

    if(current_weight > target_weight)
        return;
    if(offset >= all_pkgs.size())
        return;
    if(pkg_bunch.size() >= min_package_qty)
        return;

    for(size_t i = offset; i < all_pkgs.size(); i++)
    {
        if(current_weight + all_pkgs[i] > target_weight)
            break; // sorted, so remaining will fail also

        current_weight += all_pkgs[i];
        pkg_bunch.push_back(all_pkgs[i]);

        select_pkg(i + 1);

        current_weight -= all_pkgs[i];
        pkg_bunch.pop_back();
    }

}

uint64_t find_min_quant_en(vector<uint32_t> packages)
{
    pkg_bunches.clear();
    pkg_bunch.clear();
    all_pkgs = packages;
    sort(all_pkgs.begin(), all_pkgs.end());
    target_weight = accumulate(all_pkgs.begin(), all_pkgs.end(), 0ULL) / 3;
    current_weight = 0;
    min_package_qty = ~0L;

    select_pkg(0);

    // calc for sets with min packages
    uint64_t min_quantum_entanglement = ~0LL;
    for(size_t i = 0; i < pkg_bunches.size(); i ++)
    {
        uint64_t quantum_entanglement = 1;
        for(auto pkg : pkg_bunches[i])
            quantum_entanglement *= pkg;
        if( quantum_entanglement < min_quantum_entanglement)
            min_quantum_entanglement = quantum_entanglement;
    }

    return min_quantum_entanglement;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        vector<uint32_t> package_weight;

        //parse lines
        string line;
        while(getline(infile, line))
        {
            package_weight.push_back(stoul(line));
        }

        auto result = find_min_quant_en(package_weight);
        cout << "a) min quant entangle: " << result << "\n";

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
