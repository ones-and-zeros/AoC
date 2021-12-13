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

#define cycle_qty 100

        cout << "before any steps:" << endl;
        print_map();
        for(size_t i = 0; i < cycle_qty; i++)
        {
            update_map();

            cout << "After step " << i+1 << ":" << endl;
            print_map();
            cout << endl;
        }

        cout << "A = total flashes: " << tally << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
