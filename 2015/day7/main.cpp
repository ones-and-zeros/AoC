#include <bits/stdc++.h>
using namespace std;

enum class Opcode { onot = 0, oand, oor, olshift, orshift, oset };

vector<string> opcode_text{{"NOT"}, {"AND"}, {"OR"}, {"LSHIFT"}, {"RSHIFT"}, {"SET"}};

struct Command{
    Command(Opcode opcode,  string operand_a, string operand_b, string result)
    : opcode{opcode}, op_a{operand_a}, op_b{operand_b}, result{result}
    {
    }
    Opcode opcode;
    string op_a;
    string op_b;
    string result;
};

map<string, uint16_t> connect_wires(vector<Command> commands)
{
    map<string, uint16_t> wires;

    vector<bool> completed(commands.size(), false);

    bool in_progress = true;
    while(in_progress)
    {
        for(size_t i = 0; i < commands.size(); i++)
        {
            if(completed[i])
                continue;

            Command& c = commands[i];
            uint16_t op_a_val = 0;
            uint16_t op_b_val = 0;

            if(c.op_a.size())
            {
                if(!isalpha(c.op_a[0]))
                {
                    op_a_val = stoi(c.op_a);
                }
                else
                {
                    auto it = wires.find(c.op_a);
                    if(it == wires.end())
                        continue; //wire val not known yet
                    op_a_val = wires[c.op_a];
                }
            }
            if(c.op_b.size())
            {
                if(!isalpha(c.op_b[0]))
                {
                    op_b_val = stoi(c.op_b);
                }
                else
                {
                    auto it = wires.find(c.op_b);
                    if(it == wires.end())
                        continue; //wire val not known yet
                    op_b_val = wires[c.op_b];
                }
            }

            switch(c.opcode)
            {
                case Opcode::oset:
                    wires[c.result] = op_a_val;
                    break;
                case Opcode::onot:
                    wires[c.result] = ~op_a_val;
                    break;
                case Opcode::oand:
                    wires[c.result] = op_a_val & op_b_val;
                    break;
                case Opcode::oor:
                    wires[c.result] = op_a_val | op_b_val;
                    break;
                case Opcode::olshift:
                    wires[c.result] = op_a_val << op_b_val;
                    break;
                case Opcode::orshift:
                    wires[c.result] = op_a_val >> op_b_val;
                    break;
            }
            
            completed[i] = true;
        }

        if(completed.end() == find(completed.begin(), completed.end(), false))
            in_progress = false;
    }

    return wires;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Command> commands;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss(line);
            string word_1, word_2;
            iss >> word_1;
            iss >> word_2;

            if(word_2 == "->")
            {
                string result;
                iss >> result;
                commands.push_back({Opcode::oset, word_1, "", result});
                continue;
            }

            if(word_1 == opcode_text[static_cast<size_t>(Opcode::onot)])
            {
                string arrow, result;
                iss >> arrow;
                iss >> result;

                commands.push_back({Opcode::onot, word_2, "", result});
                continue;
            }

            bool valid = false; 
            for(size_t i = 0; i < opcode_text.size(); i++)
            {
                if(word_2 == opcode_text[i])
                {
                    string word_3, arrow, result;
                    iss >> word_3;
                    iss >> arrow;
                    iss >> result;
                    commands.push_back({static_cast<Opcode>(i), word_1, word_3, result});
                    valid = true;
                    continue;
                }
            }

            if(!valid)
                throw range_error("invalid parsing");
        }

        // for(auto c: commands)
        // {
        //     cout << opcode_text[static_cast<size_t>(c.opcode)];
        //     cout << ": " << c.op_a << ", " << c.op_b << " = " << c.result << "\n";
        // }

        map<string, uint16_t> wires = connect_wires(commands);
        // for(auto w : wires)
        //     cout << w.first << ": " << w.second << "\n";
        cout << "a) wire a: " << wires["a"] << "\n";

        // edit wire "b" to have value that was just calculate for wire "a",
        // then recalc "a"
        for(auto& c : commands)
        {
            if(c.result == "b")
            {
                c.opcode = Opcode::oset;
                c.op_a = to_string(wires["a"]);
                break;
            }
        }
        wires = connect_wires(commands);
        cout << "b) wire a: " << wires["a"] << "\n";

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
