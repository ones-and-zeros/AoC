#include <bits/stdc++.h>
using namespace std;

enum class Opcode{
    hlf = 0,
    tpl,
    inc,
    jmp,
    jie,
    jio
};

vector<string> opcode_txt{
    {"hlf"},
    {"tpl"},
    {"inc"},
    {"jmp"},
    {"jie"},
    {"jio"}
};

constexpr size_t register_qty{2};
constexpr char reg_not_used{'x'};

vector<uint32_t> processor_reg(register_qty, {0});

struct Inst{
    Inst(Opcode opcode, char reg, int32_t offset = 0)
    : opcode{opcode}, reg{reg}, offset{offset}
    {
    }
    Opcode opcode;
    char reg;
    int32_t offset;
};

int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<Inst> program;

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        //parse lines
        string line;
        while(getline(infile, line))
        {
            istringstream iss{line};
            string op_s;
            iss >> op_s;
            auto it = find(opcode_txt.begin(), opcode_txt.end(), op_s);
            if(it == opcode_txt.end())
                throw domain_error("in parse - unexpected opcode 1");
            Opcode opcode = static_cast<Opcode>(it - opcode_txt.begin());
            switch(opcode)
            {
                case Opcode::hlf:
                case Opcode::tpl:
                case Opcode::inc:
                {
                    char reg_c;
                    iss >> reg_c;
                    if(reg_c < 'a' || reg_c > 'b')
                        throw domain_error("in parse - unexpected reg 1");
                        program.push_back({opcode, reg_c});
                    break;
                }
                case Opcode::jmp:
                {
                    int32_t offset;
                    iss >> offset;
                        program.push_back({opcode, reg_not_used, offset});
                    break;
                }
                case Opcode::jie:
                case Opcode::jio:
                {
                    char reg_c;
                    iss >> reg_c;
                    if(reg_c < 'a' || reg_c > 'b')
                        throw domain_error("in parse - unexpected reg 2");
                    char comma;
                    iss >> comma;
                    if(comma != ',')
                        throw domain_error("in parse - expected comma");
                    int32_t offset;
                    iss >> offset;
                        program.push_back({opcode, reg_c, offset});
                    break;
                }
                default:
                    throw domain_error("in parse - unexpected opcode 2");
            }
        }

        for(auto inst : program)
        {
            cout << opcode_txt[static_cast<size_t>(inst.opcode)] << ", reg: "
                 << string(1, inst.reg) << ", os: " << inst.offset << "\n";  
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
