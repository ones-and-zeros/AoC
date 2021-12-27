#include <bits/stdc++.h>
using namespace std;

constexpr size_t deviation = 50;
constexpr size_t axis_range = deviation * 2 + 1; // +/- deviation, +1 for zero

size_t idx_offset(int pos) { return pos + deviation; }


struct Move{
    Move(bool turn_to, int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
    : turn_to{turn_to},
      x_min{x_min},
      x_max{x_max},
      y_min{y_min},
      y_max{y_max},
      z_min{z_min},
      z_max{z_max}
    {

    }
    bool turn_to;
    int x_min;
    int x_max;
    int y_min;
    int y_max;
    int z_min;
    int z_max;
};

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Move> moves;

        //parse lines
        while(getline(infile, line))
        {
            bool turn_to = ("on" == line.substr(0, 2));

            istringstream iss(line);
            string junk, x_min, x_max, y_min, y_max, z_min, z_max;
            getline(iss, junk, '=');
            getline(iss, x_min, '.');
            getline(iss, junk, '.');
            getline(iss, x_max, ',');

            getline(iss, junk, '=');
            getline(iss, y_min, '.');
            getline(iss, junk, '.');
            getline(iss, y_max, ',');

            getline(iss, junk, '=');
            getline(iss, z_min, '.');
            getline(iss, junk, '.');
            getline(iss, z_max, ',');

            moves.push_back( {turn_to, stoi(x_min), stoi(x_max), stoi(y_min), stoi(y_max), stoi(z_min), stoi(z_max)} );
        }

        map<pair<int,pair<int,int>>, bool> region;
        for(auto m : moves)
        {
            for(int x = m.x_min; x <= m.x_max; x++)
            {
                if(x < -deviation && x > deviation)
                    continue;

                for(int y = m.y_min; y <= m.y_max; y++)
                {
                    if(y < -deviation && y > deviation)
                        continue;

                    for(int z = m.z_min; z <= m.z_max; z++)
                    {
                        if(z < -deviation && z > deviation)
                            continue;
                        region[{x, {y, z}}] = m.turn_to;
                    }
                }
            }
        }
        size_t tally = 0;
        for(auto b : region)
            if(b.second)
                tally++;
        cout << " a- on tally: " << tally << '\n';


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
