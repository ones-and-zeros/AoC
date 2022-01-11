#include <bits/stdc++.h>
using namespace std;

struct Cube{
    Cube(int x_min, int x_max, int y_min, int y_max, int z_min, int z_max)
    :   x_min{x_min},
        x_max{x_max},
        y_min{y_min},
        y_max{y_max},
        z_min{z_min},
        z_max{z_max}
    { }
    int64_t x_min;
    int64_t x_max;
    int64_t y_min;
    int64_t y_max;
    int64_t z_min;
    int64_t z_max;
};

struct Move{
    Move(bool turn_to, Cube cube)
    : turn_on{turn_to}, cube{cube}
    {

    }
    Cube cube;
    bool turn_on;
};

uint64_t calc_zone(vector<Move>& moves, Cube zone)
{
    uint64_t tally = 0;

    vector<Move> moves_done;
    for(auto m : moves)
    {
        if( (m.cube.x_min > zone.x_max) ||
            (m.cube.x_max < zone.x_min) ||
            (m.cube.y_min > zone.y_max) ||
            (m.cube.y_max < zone.y_min) ||
            (m.cube.z_min > zone.z_max) ||
            (m.cube.z_max < zone.z_min) )
        {
            //out of bounds
            continue;
        }
        if( m.cube.x_min < zone.x_min)
            m.cube.x_min = zone.x_min;
        if( m.cube.x_max > zone.x_max)
            m.cube.x_max = zone.x_max;

        if( m.cube.y_min < zone.y_min)
            m.cube.y_min = zone.y_min;
        if( m.cube.y_max > zone.y_max)
            m.cube.y_max = zone.y_max;

        if( m.cube.z_min < zone.z_min)
            m.cube.z_min = zone.z_min;
        if( m.cube.z_max > zone.z_max)
            m.cube.z_max = zone.z_max;

        uint64_t tally_prev = calc_zone(moves_done, m.cube);
        moves_done.push_back(m); //for next time

        if(m.turn_on)
        {
            uint64_t tally_this = (1 + m.cube.x_max - m.cube.x_min) *
                                    (1 + m.cube.y_max - m.cube.y_min) *
                                    (1 + m.cube.z_max - m.cube.z_min);
            tally += (tally_this - tally_prev);
        }
        else
        {
            if(tally_prev > tally)
                throw range_error("tally prev too big");
            tally -= tally_prev;
        }
    }

    return tally;
}

uint64_t calc_total(vector<Move>& moves)
{
    uint64_t tally = 0;

    vector<Move> moves_done;
    for(auto m : moves)
    {
        uint64_t tally_prev = calc_zone(moves_done, m.cube);
        moves_done.push_back(m); //for next time

        if(m.turn_on)
        {
            uint64_t tally_this = (1 + m.cube.x_max - m.cube.x_min) *
                                  (1 + m.cube.y_max - m.cube.y_min) *
                                  (1 + m.cube.z_max - m.cube.z_min);
            tally += (tally_this - tally_prev);
        }
        else
        {
            if(tally_prev > tally)
                throw range_error("tally prev too big");
            tally -= tally_prev;
        }
    }

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

            moves.push_back( {turn_to, Cube(stoi(x_min), stoi(x_max), stoi(y_min), stoi(y_max), stoi(z_min), stoi(z_max))} );
        }


        cout << " a - init tally: " << calc_zone(moves, {-50,50,-50,50,-50,50}) << '\n';
        cout << " b - full tally: " << calc_total(moves) << '\n';


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
