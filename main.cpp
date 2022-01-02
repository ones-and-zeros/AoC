#include <bits/stdc++.h>
using namespace std;


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

        using Point = pair<int,int>;
        map<Point, uint64_t> houses;
        
        int x = 0;
        int y = 0;
        houses[{x,y}]++; //starting house gets a present
        for(auto m : moves)
        {
            switch(m)
            {
                case '<':
                    y--;
                    break;
                case '>':
                    y++;
                    break;
                case '^':
                    x++;
                    break;
                case 'v':
                    x--;
                    break;            
            }
            houses[{x,y}]++;
        }

        cout << " a - qty of houses: " << houses.size() << "\n"; 

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
