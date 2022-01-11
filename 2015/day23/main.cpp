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

struct Inst{
    Inst(Opcode opcode, char reg, int32_t offset = 0)
    : opcode{opcode}, reg{reg}, offset{offset}
    {
    }
    Opcode opcode;
    char reg;
    int32_t offset;
};

vector<uint64_t> run_prog(vector<Inst> program, uint64_t a_init)
{
    vector<uint64_t> reg(register_qty, {0});

    reg[0] = a_init;

    for(size_t i = 0; i < program.size(); i++)
    {
        Inst inst = program[i];

        switch(inst.opcode)
        {
            case Opcode::hlf:
            {
                reg[inst.reg - 'a'] >>= 1;
                break;
            }
            case Opcode::tpl:
            {
                reg[inst.reg - 'a'] *= 3;
                break;
            }
            case Opcode::inc:
            {
                reg[inst.reg - 'a']++;
                break;
            }
            case Opcode::jmp:
            {
                i += inst.offset;
                i--; // will incremnt this back in for loop
                break;
            }
            case Opcode::jie:
            {
                if(0 == (reg[inst.reg - 'a'] & 1UL))
                {
                    i += inst.offset;
                    i--; // will incremnt this back in for loop
                }
                break;
            }
            case Opcode::jio:
            {
                if(1 == reg[inst.reg - 'a'])
                {
                    i += inst.offset;
                    i--; // will incremnt this back in for loop
                }
                break;
            }
        }
    }

    return reg;
}

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

        // for(auto inst : program)
        // {
        //     cout << opcode_txt[static_cast<size_t>(inst.opcode)] << ", reg: "
        //          << string(1, inst.reg) << ", os: " << inst.offset << "\n";  
        // }

        auto result = run_prog(program, 0);
        cout << "a) reg b: " <<  result[1] << "\n";

        result = run_prog(program, 1);
        cout << "b) init a=1 reg b: " <<  result[1] << "\n";

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
