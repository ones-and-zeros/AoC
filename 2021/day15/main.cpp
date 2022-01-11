#include <bits/stdc++.h>
using namespace std;


unsigned long calc_path_a(vector<string>& grid)
{
    size_t x_qty = grid.size();
    size_t y_qty = grid[0].size();

    // create border row\column around whole grid, initialized with MAX (worst case)
    vector<vector<unsigned long>> sum(x_qty + 2, vector<unsigned long>(y_qty + 2,  ~(unsigned long)0));

    // seed a starting point as 0. (could have been at location {1,0} either)
    sum[0][1] = 0;

    bool update = true;
    while(update)
    {
        update = false;

        for(size_t x=1; x<=x_qty; x++)
        {
            for(size_t y=1; y<=y_qty; y++)
            {
                unsigned long temp = min( min(sum[x-1][y], sum[x+1][y]),
                                          min(sum[x][y+1], sum[x][y-1]));
                temp += (grid[x-1][y-1] - '0');
                if(sum[x][y] != temp)
                {
                    sum[x][y] = temp;
                    update = true;
                }
            }
        }
    }

    return sum[x_qty][y_qty] - sum[1][1];
}

unsigned long calc_path_b(vector<string>& grid)
{
    size_t x_qty = grid.size() * 5;
    size_t y_qty = grid[0].size() * 5;

    // create border row\column around whole grid, initialized with MAX (worst case)
    vector<vector<unsigned long>> sum(x_qty + 2, vector<unsigned long>(y_qty + 2,  ~(unsigned long)0));

    // seed a starting point as 0. (could have been at location {1,0} either)
    sum[0][1] = 0;

    bool update = true;

    while(update)
    {
        update = false;

        for(size_t x=1; x<=x_qty; x++)
        {
            for(size_t y=1; y<=y_qty; y++)
            {
                size_t x_grid = x - 1;
                size_t y_grid = y - 1;

                unsigned long val = grid[x_grid % grid.size()][y_grid % grid[0].size()] - '0';
                val += x_grid / grid.size();
                val += y_grid / grid[0].size();
                // wrap from 9 to 1
                if(val > 9)
                    val -= 9;
                // add value to lowest adjacent (above, below, left, or right)
                val += min( min(sum[x-1][y], sum[x+1][y]),
                            min(sum[x][y+1], sum[x][y-1]));

                if(sum[x][y] != val)
                {
                    sum[x][y] = val;
                    update = true;
                }
            }
        }
    }

    return sum[x_qty][y_qty] - sum[1][1];
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<string> grid;

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        string input_s;

        //parse lines
        while(getline(infile, file_line))
        {
            // istringstream iss{file_line};
            // string key, val, junk;
            // getline(iss, key, ' ');

            grid.push_back(file_line);
        }

        for(auto row : grid)
            cout << row << endl;
        cout << endl;

        size_t answer_a = calc_path_a(grid);
        cout << " answer a = " << answer_a << endl;
        cout << endl;

        size_t answer_b = calc_path_b(grid);
        cout << " answer b = " << answer_b << endl;
        cout << endl;

        // for(size_t x=1; x<=x_qty; x++)
        // {
        //     for(size_t y=1; y<=y_qty; y++)
        //         cout << sum[x][y] << "\t";
        //     cout << endl;
        // }

        infile.close();
    }

    // execution time
    // auto stop = chrono::high_resolution_clock::now();
    // auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    // cout << "sec: " << (duration.count() / 1000000) << 
    //                 "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    // cout << endl;
}
