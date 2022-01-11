#include <bits/stdc++.h>
using namespace std;

constexpr char east_c{'>'};
constexpr char south_c{'v'};
constexpr char unocc{'.'};

vector<string> sea_floor;
size_t x_len;
size_t y_len;
uint64_t settle_count = 0;

bool settle()
{
    bool movement = false;

    settle_count++;

    // move east
    for(size_t y = 0; y < y_len; y++)
    {
        string temp_row;
        for(size_t x = 0; x < x_len; x++)
        {
            if( sea_floor[y][x] == unocc && 
                sea_floor[y][x > 0 ? x - 1 : x_len - 1] == east_c)
            {
                temp_row += east_c;
                movement = true;
            }
            else if( sea_floor[y][x] == east_c && 
                     sea_floor[y][(x + 1) % x_len] == unocc)
            {
                temp_row += unocc;
            }
            else
                temp_row += sea_floor[y][x];
        }
        sea_floor[y] = temp_row;
    }

    // cout << "east " << settle_count << ":\n";
    // for(auto r : sea_floor)
    //     cout << r << '\n';
    // cout << '\n';

    // move south
    for(size_t x = 0; x < x_len; x++)
    {
        string temp_column;
        for(size_t y = 0; y < y_len; y++)
        {
            if( sea_floor[y][x] == unocc && 
                sea_floor[y > 0 ? y - 1 : y_len - 1][x] == south_c)
            {
                temp_column += south_c;
                movement = true;
            }
            else if( sea_floor[y][x] == south_c && 
                     sea_floor[(y + 1) % y_len][x] == unocc)
            {
                temp_column += unocc;
            }
            else
                temp_column += sea_floor[y][x];
        }
        for(size_t y = 0; y < y_len; y++)
            sea_floor[y][x] = temp_column[y];
    }

    // cout << "south " << settle_count << ":\n";
    // for(auto r : sea_floor)
    //     cout << r << '\n';
    // cout << '\n';

    return movement;
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
            sea_floor.push_back(line);

        y_len = sea_floor.size();
        x_len = sea_floor[0].size();

        // for(auto r : sea_floor)
        //     cout << r << '\n';
        // cout << '\n';

        while(settle());

        cout << " a - moves to settle: " << settle_count << '\n';

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
