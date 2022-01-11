#include <bits/stdc++.h>
using namespace std;

//#define PRINT_GRIDS
//#define VISUALIZE

constexpr char light_on{'#'};
constexpr char light_off{'.'};

void delay_mS(uint64_t time_mS)
{
    auto stop = chrono::high_resolution_clock::now() + chrono::milliseconds(time_mS);
    while(stop > chrono::high_resolution_clock::now())
        ;
}

ostream& operator<<(ostream& os, vector<string> grid)
{
    for(auto row : grid)
        os << row << "\n";
    return os;
}

uint32_t animate(vector<string> grid, size_t step_qty, bool corners_always_on)
{
    size_t x_qty = grid.size();
    size_t y_qty = grid[0].size();

    if(corners_always_on)
    {
        grid[0      ][0      ] = light_on;
        grid[0      ][y_qty-1] = light_on;
        grid[x_qty-1][0      ] = light_on;
        grid[x_qty-1][y_qty-1] = light_on;
    }

#ifdef PRINT_GRIDS
#ifdef VISUALIZE
    cout << "\033[2J\033[1;1H"; //clear terminal screen
#endif
    cout << "original:\n";
    cout << grid << "\n";
#ifdef VISUALIZE
        delay_mS(1000);
#endif
#endif

    for(size_t step = 1; step <= step_qty; step++)
    {
        vector<string> temp_grid;
        for(size_t x = 0; x < x_qty; x++)
        {
            string row;
            for(size_t y = 0; y < x_qty; y++)
            {
                size_t adj = 0;
                if(x>0 && y>0 &&                grid[x - 1][y - 1] == light_on)
                    adj++;
                if(x>0 &&                       grid[x - 1][y    ] == light_on)
                    adj++;
                if(x>0 && y+1<y_qty &&          grid[x - 1][y + 1] == light_on)
                    adj++;
                if(y>0 &&                       grid[x    ][y - 1] == light_on)
                    adj++;
                if(y+1<y_qty &&                 grid[x    ][y + 1] == light_on)
                    adj++;
                if(x+1<x_qty && y>0 &&          grid[x + 1][y - 1] == light_on)
                    adj++;
                if(x+1<x_qty &&                 grid[x + 1][y    ] == light_on)
                    adj++;
                if(x+1<x_qty && y+1<y_qty &&    grid[x + 1][y + 1] == light_on)
                    adj++;

                if(grid[x][y] == light_on)
                    row += (adj == 2 || adj == 3) ? light_on : light_off;
                else if(grid[x][y] == light_off)
                    row += (adj == 3) ? light_on : light_off;
                else
                    throw domain_error("unexpected light val");
            }
            temp_grid.push_back(row);
        }
        grid = temp_grid;

        if(corners_always_on)
        {
            grid[0      ][0      ] = light_on;
            grid[0      ][y_qty-1] = light_on;
            grid[x_qty-1][0      ] = light_on;
            grid[x_qty-1][y_qty-1] = light_on;
        }

#ifdef PRINT_GRIDS
#ifdef VISUALIZE
        delay_mS(200);
        cout << "\033[2J\033[1;1H"; //clear terminal screen
#endif
        cout << "step " << step << ":\n";
        cout << grid << "\n";
#endif
    }

#ifdef VISUALIZE
        delay_mS(200);
        cout << "\033[2J\033[1;1H"; //clear terminal screen
        delay_mS(200);
#endif

    uint32_t tally = 0;
    for(auto row : grid)
        for(auto light : row)
            if(light == light_on)
                tally++;

    return tally;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> input_grid;

        //parse lines
        while(getline(infile, line))
        {
            input_grid.push_back(line);
        }

        if(input_grid.size() == 6)
        {
            auto lights_on = animate(input_grid, 4, false);
            cout << "s-a) on after 4 cycles: " << lights_on << "\n";
            lights_on = animate(input_grid, 5, true);
            cout << "s-b) corners stuck on, after 5 cycles: " << lights_on << "\n";
        }
        else if(input_grid.size() == 100)
        {
            auto lights_on = animate(input_grid, 100, false);
            cout << "a) on after 100 cycles: " << lights_on << "\n";
            lights_on = animate(input_grid, 100, true);
            cout << "b) corners stuck on, after 100 cycles: " << lights_on << "\n";
        }
        else
            throw domain_error("unexpected grid size");

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
