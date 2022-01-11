#include <bits/stdc++.h>
using namespace std;

map<string,uint32_t> measured {
    {"children", 3},
    {"cats", 7},
    {"samoyeds", 2},
    {"pomeranians", 3},
    {"akitas", 0},
    {"vizslas", 0},
    {"goldfish", 5},
    {"trees", 3},
    {"cars", 2},
    {"perfumes", 1}
};

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<map<string,uint32_t>> aunts;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss{line};
            string name;
            getline(iss, name, ':');
            
            char c = ' ';
            while(c == ' ')
                iss >> c;
            iss.putback(c);

            string title;
            map<string,uint32_t> compounds;
            while( getline(iss, title, ':') )
            {
                int val;
                iss >> val;
                compounds.insert({title, val});
                string comma;
                getline(iss, title, ' ');
            }

            aunts.push_back({compounds});
        }

        for(size_t a = 0; a < aunts.size(); a++)
        {
            bool found = true;
            for(auto c : aunts[a])
            {
                if(measured[c.first] == c.second)
                    continue;
                found = false;
                break;
            }
            if(found)
            {
                cout << "a) aunt: Sue " << a + 1 << "\n";
            }
        }

        for(size_t a = 0; a < aunts.size(); a++)
        {

            //debug
            static size_t pos_debug = 476;
            if(a == pos_debug)
            {
                asm("nop");
            }

            bool found = true;
            for(auto c : aunts[a])
            {
                if(c.first == "cats" || c.first == "trees")
                {
                    if(c.second > measured[c.first])
                        continue;
                }
                else if(c.first == "pomeranians" || c.first == "goldfish")
                {
                    if(c.second < measured[c.first])
                        continue;
                }
                else if(c.second == measured[c.first])
                {
                    continue;
                }
                found = false;
                break;
            }
            if(found)
            {
                cout << "b) aunt: Sue " << a + 1 << "\n";
            }
        }

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
