#include <bits/stdc++.h>
using namespace std;

struct Reindeer{
    Reindeer(string name, uint32_t rate_km_s, uint32_t on_time, uint32_t off_time)
    : name{name}, rate{rate_km_s}, on{on_time}, period{on_time + off_time}
    {
    }
    string name;
    uint32_t rate;
    uint32_t on;
    uint32_t period;
};

// uint64_t distance(Reindeer reindeer, uint32_t duration)
// {
//     //full cycles first
//     uint64_t distance = (duration / reindeer.period) * (reindeer.rate * reindeer.on);
//     //partial cycle remaining
//     uint64_t time_remaining = duration % reindeer.period;
//     distance += reindeer.rate * ((time_remaining < reindeer.on) ? time_remaining : reindeer.on);
    
//     return distance; 
// }

vector<pair<string,uint64_t>> race(vector<Reindeer> reindeer, uint32_t duration, bool points_used)
{
    vector<pair<string,uint64_t>> results;
    for(auto deer : reindeer)
        results.push_back({deer.name, 0});

    vector<pair<string,uint64_t>> points;
    if(points_used)
        for(auto deer : reindeer)
            points.push_back({deer.name, 0});

    for(uint32_t sec = 0; sec < duration; sec++)
    {
        for(size_t d = 0; d < reindeer.size(); d++)
        {
            if( (sec % reindeer[d].period) < reindeer[d].on )
            {
                results[d].second += reindeer[d].rate;
            }
        }

        if(!points_used)
            continue;
        uint64_t lead_dist = 0;
        for(auto dist : results)
            if(dist.second > lead_dist)
                lead_dist = dist.second;
        for(size_t dd = 0; dd < reindeer.size(); dd++)
            if(results[dd].second== lead_dist)
                points[dd].second++;
    }

    if(points_used)
        results = points;

    sort(results.begin(), results.end(),
        [](pair<string, uint64_t> a, pair<string, uint64_t> b){ return a.second > b.second;});

    return results;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Reindeer> reindeer;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss{line};
            string name;
            iss >> name;

            string word;
            iss >> word;
            while(!isdigit(word[0]))
                iss >> word;
            uint32_t rate = stoi(word);

            iss >> word;
            while(!isdigit(word[0]))
                iss >> word;
            uint32_t on_time = stoi(word);

            iss >> word;
            while(!isdigit(word[0]))
                iss >> word;
            uint32_t off_time = stoi(word);
            
            reindeer.push_back({name, rate, on_time, off_time});
        }

        // for(auto deer : reindeer)
        // {
        //     cout << deer.name << "\n  " << deer.rate << " km/s\n" <<
        //       "  on  = " << deer.on << "\n  off = " << (deer.period - deer.on) << "\n";
        // }

//        race(reindeer)

        auto results = race(reindeer, 1000, false);
        cout << "s) 1000 sec : " << results[0].first << " " << results[0].second << " km\n";
        results = race(reindeer, 2503, false);
        cout << "a) 2503 sec : " << results[0].first << " " << results[0].second << " km\n";
        cout << "\n";

        results = race(reindeer, 1000, true);
        cout << "s) 1000 sec : " << results[0].first << " " << results[0].second << " km\n";
        results = race(reindeer, 2503, true);
        cout << "b) 2503 sec : " << results[0].first << " " << results[0].second << " km\n";

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
