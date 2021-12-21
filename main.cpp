#include <bits/stdc++.h>
using namespace std;

using Point = pair<int,int>; //x,y position
using Velocity = pair<int,int>; //x=fwd,y=up

int target_x_min;
int target_x_max;
int target_y_min;
int target_y_max;
int max_y = 0;

bool hit_target(Velocity velocity)
{
    Point current = {0,0};
    int max_y_current = 0;

    while(1)
    {
        current.first += velocity.first;
        current.second += velocity.second;

        if(current.second > max_y_current)
            max_y_current = current.second;

        if(velocity.first > 0)
            velocity.first--;
        if(velocity.first < 0)
            velocity.first++;
        velocity.second--;

        if(current.first > target_x_max)
            break;
        if(current.second < target_y_min)
            break;
        
        if(current.first < target_x_min)
            continue;
        if(current.second > target_y_max)
            continue;

        if(max_y_current > max_y)
            max_y = max_y_current;

        return true;
    }
    return false;
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
            istringstream iss{file_line};
            string x_min, x_max, y_min, y_max, junk;
            getline(iss, junk, '=');
            getline(iss, x_min, '.');
            getline(iss, junk, '.');
            getline(iss, x_max, ',');
            getline(iss, junk, '=');
            getline(iss, y_min, '.');
            getline(iss, junk, '.');
            getline(iss, y_max);

            target_x_min = stoi(x_min);
            target_x_max = stoi(x_max);
            target_y_min = stoi(y_min);
            target_y_max = stoi(y_max);
        }

        // vector<Velocity> vel = { {7,2}, {6,3}, {9,0}, {17,-4} };
        // for(auto v : vel)
        // {
        //     bool hit = hit_target(v);
        //     cout << "{" << v.first << ", " << v.second << "} = " << (hit ? "hit" : "miss") << endl;
        // }

        for(int x = 0; x < 1000; x++)
            for(int y = 0; y < 1000; y++)
                hit_target({x,y});
        cout << "a - max height = " << max_y << endl;

        cout << endl;
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
