#include <bits/stdc++.h>
using namespace std;

set<string> cities;
map<pair<string,string>, uint32_t> city_distance;

set<string> travel_route;
uint64_t travel_dist = 0;

pair<set<string>,uint64_t> shortest_route;
pair<set<string>,uint64_t> longest_route;

void next_city(string city)
{
    if(travel_route.size() == cities.size())
    {
        if(travel_dist > longest_route.second)
        {
            longest_route.first = travel_route;
            longest_route.second = travel_dist;
        }
        if(travel_dist < shortest_route.second)
        {
            shortest_route.first = travel_route;
            shortest_route.second = travel_dist;
        }
        return;
    }

    for(auto dest : cities)
    {
        if(travel_route.end() != travel_route.find(dest))
            continue;
        travel_dist += city_distance[{city, dest}];
        travel_route.insert(dest);
        next_city(dest);  //start with each city
        travel_route.erase(dest);
        travel_dist -= city_distance[{city, dest}];
    }
}

void travel()
{
    travel_dist = 0;
    travel_route.clear();
    shortest_route.first.clear();
    shortest_route.second = ~0ULL;
    longest_route.first.clear();
    longest_route.second = 0;

    for(auto city : cities)
    {
        travel_route.insert(city);
        next_city(city);  //start with each city
        travel_route.erase(city);
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

        vector<string> s_in;

        //parse lines
        while(getline(infile, line))
        {
            s_in.push_back(line);
            istringstream iss{line};
            string city_a, to, city_b, eqsign, dist;
            iss >> city_a >> to >> city_b >> eqsign >> dist;
            if(to != "to" || eqsign != "=")
                throw range_error("parsing error");

            cities.insert(city_a);
            cities.insert(city_b);
            city_distance.insert({{city_a, city_b}, stoul(dist)});
            city_distance.insert({{city_b, city_a}, stoul(dist)});
        }

        // for(auto c : cities)
        //     cout << c << "\n";

        travel();
        
        cout << "a) shortest trip: " << shortest_route.second << "\n";
        for(auto city : shortest_route.first)
            cout << "  " << city << "\n";        
        cout << '\n';

        cout << "b) longest trip: " << longest_route.second << "\n";
        for(auto city : longest_route.first)
            cout << "  " << city << "\n";        
        cout << '\n';

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
