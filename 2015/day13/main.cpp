#include <bits/stdc++.h>
using namespace std;

bool seat_myself = false;
int64_t happiness;
int64_t best_seated_happiness;
vector<string> seat_order;
vector<string> best_seat_order;
set<string> people_to_seat;
map<pair<string,string>,int> master_pref;

int64_t how_happy(const string name_1, const string name_2)
{
    if( master_pref.find({pair<string, string>{name_1, name_2}}) == master_pref.end() )
        throw range_error("expected pref 1");
    if( master_pref.find({pair<string, string>{name_2, name_1}}) == master_pref.end() )
        throw range_error("expected pref 1");
    return master_pref[{name_1, name_2}] + master_pref[{name_2, name_1}];
}

void seat_next(string person)
{
    if(seat_order.size() == people_to_seat.size())
    {
        //last to be seated, so sitting next to first to sit
        int64_t temp = how_happy(seat_order[seat_order.size() - 1], seat_order[0]);

        if(seat_myself)
            temp = 0;

        happiness += temp;
        if(happiness > best_seated_happiness)
        {
            // found new best
            best_seated_happiness = happiness;
            best_seat_order = seat_order;
        }
        happiness -= temp;
        return;
    }

    for(auto next : people_to_seat)
    {
        if(seat_order.end() != find(seat_order.begin(), seat_order.end(), next))
            continue;
        seat_order.push_back(next);
        int64_t temp = how_happy(seat_order[seat_order.size() - 1], seat_order[seat_order.size() - 2]);
        happiness += temp;
        seat_next(next);
        happiness -= temp;
        seat_order.pop_back();        
    }
}

pair<int64_t, vector<string>> ideal_seating(map<pair<string,string>,int> seat_pref, bool include_myself)
{
    master_pref = seat_pref;
    seat_myself = include_myself;

    for(auto pref : master_pref)
        people_to_seat.insert(pref.first.first);

    happiness = 0;
    best_seated_happiness = 0;
    seat_order.clear();
    for(auto person : people_to_seat)
    {
        seat_order.push_back(person);
        seat_next(person);
        seat_order.pop_back();
    }

    return {best_seated_happiness, best_seat_order};
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        set<string> people;
        map<pair<string,string>,int> seat_pref;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss{line};
            string name, would, dir, val_s;
            iss >> name >> would >> dir >> val_s;
            if(would != "would")
                throw domain_error("invalid parse, expected 'would'");

            int val = 0;
            if(dir == "gain")
                val = stoi(val_s);
            else if(dir == "lose")
                val = 0 - stoi(val_s);
            else
                throw domain_error("invalid parse, expected 'gain' ot 'lose'");

            string name2;
            while(iss.rdbuf()->in_avail() != 0)
                iss >> name2;
            name2.erase(name2.end() - 1); //get rid of period

            seat_pref.insert({{name, name2}, val});
        }

        for(auto person : people)
        {
            cout << person << " prefers:\n";
            for(auto other : people)
            {
                if(person == other)
                    continue;
                cout << "  " << other << " " << seat_pref[{person, other}] << "\n";
            }
        }

        auto seating = ideal_seating(seat_pref, false);
        cout << "a) ideal seating: " << seating.first << "\n";
        // for(auto person : seating.second)
        //     cout << "  " << person << "\n";

        seating = ideal_seating(seat_pref, true);
        cout << "b) ideal w/ me: " << seating.first << "\n";

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
