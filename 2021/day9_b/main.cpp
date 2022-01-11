#include <bits/stdc++.h>
using namespace std;

//answer: 1059300 = (107 * 100 * 99)

using point = pair<int,int>;
using region = set<point>;

vector<region> regions;

void add_line(region& line_to_add)
{
    //create set of points directly above line_to_add
    region to_find;
    for(auto p : line_to_add)
        if(p.first)
            to_find.insert({p.first - 1, p.second});

    auto target = regions.end();
    bool line_added = false;
    bool regions_merged = false;
    for(auto ri = regions.begin(); ri != regions.end(); ri++)
    {
        bool found = false;
        for(auto p : to_find)
        {
            if(ri->find(p) != ri->end())
            {
                found = true;
                break;
            }
        }
        if(found)
        {
            if(target == regions.end())
            {
                target = ri;
                target->insert(line_to_add.begin(), line_to_add.end());
                line_added = true;
            }
            else
            {
                target->insert(ri->begin(),ri->end());
                ri->clear();
                regions_merged = true;
            }
        }
    }

    if(!line_added)
        regions.push_back(line_to_add);
    else if(regions_merged)
        regions.erase(remove_if(regions.begin(), regions.end(), [](region r){return r.size() == 0;}), regions.end());
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::vector<std::string> rows_s;

        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
            rows_s.push_back(file_line);

        size_t x_max = rows_s.size();
        size_t y_max = rows_s[0].size(); //assuming all rows are same length (due to input data)

        // cycle through each point on grid
        for(int x = 0; x < x_max; x++)
        {
            // collect adjacent points to form line to add
            region current;
            for(int y = 0; y < y_max; y++)
            {
                if(rows_s[x][y] != '9')
                {
                    current.insert({x,y});
                }
                else if(current.size())
                {
                    add_line(current);
                    current.clear();
                }
            }
            if(current.size())
                add_line(current);
        }

        vector<size_t> top_areas;
        for(auto r : regions)
            top_areas.push_back(r.size());
        sort(top_areas.begin(), top_areas.end(), greater<size_t>());

        size_t answer = top_areas[0] * top_areas[1] * top_areas[2];
        cout << "answer: " << answer << " = ("
             << top_areas[0] << " * " << top_areas[1] << " * " << top_areas[2] << ")" << endl;
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << endl << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}