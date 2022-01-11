#include <bits/stdc++.h>
using namespace std;

#define x_qty (rows_s.size())
#define y_qty (rows_s[0].size())

vector<string> rows_s;
size_t tally = 0;

void inc_loc(int x, int y)
{
    if(x >= 0 &&
       x < x_qty &&
       y >= 0 &&
       y < y_qty )
    {
        rows_s[x][y]++;
        if(rows_s[x][y] == ((int)'9' + 1))
        {
            inc_loc(x-1,y);
            inc_loc(x-1,y+1);
            inc_loc(x-1,y-1);
            inc_loc(x,y+1);
            inc_loc(x,y-1);
            inc_loc(x+1,y);
            inc_loc(x+1,y+1);
            inc_loc(x+1,y-1);
        }
    }
}

void update_map()
{
    for(int x = 0; x < x_qty; x++)
        for(int y = 0; y < y_qty; y++)
            inc_loc(x,y);

    for(int x = 0; x < x_qty; x++)
    {
        for(int y = 0; y < y_qty; y++)
        {
            if(rows_s[x][y] > '9')
            {
                tally++;
                rows_s[x][y] = '0';
            }
        }
    }
}

void print_map()
{
    for(int x = 0; x < x_qty; x++)
    {
        cout << rows_s[x] << endl;
    }
    cout << endl;
}

bool all_active()
{
    for(auto s : rows_s)
        for(auto c : s)
            if(c != '0')
                return false;
    return true;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        //parse lines
        while(getline(infile, file_line))
        {
            rows_s.push_back(file_line);
        }

        size_t step = 0;
        while(1)
        {
            step++;
            update_map();

            if(step == 100)
                cout << "A = total flashes after 100 steps: " << tally << endl;

            if(all_active())
                break;
        }
        cout << "B = all on after step: " << step << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
