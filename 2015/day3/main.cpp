#include <bits/stdc++.h>
using namespace std;

using Point = pair<int,int>;

void update_point(Point& point, char move)
{
    switch(move)
    {
        case 'v':
            point.first--;
            break;            
        case '^':
            point.first++;
            break;
        case '<':
            point.second--;
            break;
        case '>':
            point.second++;
            break;
    }
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        string moves;

        //parse lines
        while(getline(infile, line))
        {
            if(line.size())
                moves = line;
        }

        map<Point, uint64_t> houses;

        Point santa = {0,0};
        for(auto m : moves)
        {
            update_point(santa, m);
            houses[santa]++;
        }
        cout << " a - qty of houses: " << houses.size() << "\n"; 



        constexpr size_t real = 0;
        constexpr size_t robo = 1;
        vector<Point> santas{ {0,0}, {0,0}};
        houses.clear();
        houses[santas[real]]++; //starting house gets a present
        houses[santas[robo]]++; //starting house gets a present
        size_t s = real;
        for(auto m : moves)
        {
            update_point(santas[s], m);
            houses[santas[s]]++;
            s = (real == s) ? robo : real;
        }
        cout << " b - qty of houses: " << houses.size() << "\n"; 

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
