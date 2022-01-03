#include <bits/stdc++.h>
using namespace std;

using Point = pair<size_t, size_t>;

vector<bitset<1000>> lights(1000, bitset<1000>(0));

enum class Opcode { off = 0, on, toggle };

vector<string> opcode_text {
    {"off"},
    {"on"},
    {"tog"}
};

struct Instruct{
    Instruct(Opcode opcode, Point start, Point stop)
    : opcode{opcode}, start{start}, stop{stop}
    {        
    }
    Opcode opcode;
    Point start;
    Point stop;
};

ostream& operator<<(ostream& os, Instruct inst)
{
    os  << opcode_text[static_cast<size_t>(inst.opcode)] <<
        " {" << inst.start.first << ", " << inst.start.second << "} to {" <<
        inst.stop.first << ", " << inst.stop.second << "}";
    return os;
}

void run_instr(Instruct inst)
{
    for(size_t x = inst.start.first; x <= inst.stop.first; x++)
    {
        for(size_t y = inst.start.second; y <= inst.stop.second; y++)
        {
            if(Opcode::off == inst.opcode)
                lights[x][y] = 0;
            else if(Opcode::on == inst.opcode)
                lights[x][y] = 1;
            else if(Opcode::toggle == inst.opcode)
                lights[x][y] = !lights[x][y];
            else
                throw range_error("unexpected instruction");
        }    
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

        vector<Instruct> program;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss(line);
            string word;
            iss >> word;
            Opcode opcode;

            if( word == "toggle")
                opcode = Opcode::toggle;
            else if( word == "turn")
            {
                iss >> word;
                if(word == "on")
                    opcode = Opcode::on;
                else if(word == "off")
                    opcode = Opcode::off;
                else
                    throw range_error("Input parse error 2");
            }
            else
                throw range_error("Input parse error 1");

            size_t x,y;
            char comma;

            iss >> x;
            iss >> comma;
            iss >> y;
            Point start{x,y};

            iss >> word;
            if(word != "through")            
                throw range_error("Input parse error 3");

            iss >> x;
            iss >> comma;
            iss >> y;
            Point stop{x,y};

            program.push_back({opcode, start, stop});
        }

        // for(auto step : program)
        //     cout << step << "\n";


        for(auto instr : program)
            run_instr(instr);

        uint64_t total = 0;
        uint64_t on = 0;
        for(auto l : lights)
        {
            total += l.size();
            on += l.count();
        }
        cout << total << ": " << on << "\n";




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
