#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

using Factory_value = uint32_t;
using Bot = vector<Factory_value>;
using Bots = vector<Bot>;

using Output = vector<Factory_value>;
using Outputs = vector<Output>;

using Node_id = size_t;
using Bot_id = Node_id;
using Output_id = Node_id;

struct Placement{
    Bot_id bot_id;
    Factory_value value;
};
using Placements = vector<Placement>;

struct Relationship{
    Node_id low_id;
    Node_id high_id;
    bool low_is_output;
    bool high_is_output;
};
using Relationships = map<Bot_id, Relationship>;

using Instruction = string;
using Instructions = vector<Instruction>;

class Factory{
public:
    Factory(const Instructions & instructions)
    {
        parse_instructions(instructions);
        init_bots();
        init_outputs();
        process();
    }

    Bot_id bot_that_processes_values(Factory_value val_a, Factory_value val_b)
    {
        auto low = min(val_a, val_b); 
        auto high = max(val_a, val_b); 

        if(vals_compared_to_bot_id.end() == vals_compared_to_bot_id.find({low, high}))
            return ~0;
        return vals_compared_to_bot_id[{low, high}];
    }

    Factory_value output_value(Output_id output_id)
    {
        if(outputs[output_id].empty())
            return ~0;
        return outputs[output_id][0];
    }

private:
    void parse_instructions(const Instructions & instructions)
    {
        for(auto & i : instructions)
        {
            if(add_relationship(i))
                continue;
            if(add_placement(i))
                continue;
            throw("invalid instruction parse");
        }
    }

    void init_bots()
    {
        bots = Bots{max_bot_id + 1, Bot{}};
    }

    void init_outputs()
    {
        outputs = Outputs{max_output_id + 1, Output{}};
    }

    bool add_placement(const Instruction instruction)
    {
        if("value " != instruction.substr(0, 6))
            return false;

        istringstream iss{instruction};
        Factory_value value;
        Bot_id bot_id;
        string word;

        iss >> word; //"value"
        iss >> value;

        iss >> word; //"goes"
        iss >> word; //"to"
        iss >> word; //"bot"
        iss >> bot_id;

        placements.push_back({bot_id, value});
        return true;
    }
    bool add_relationship(const Instruction instruction)
    {
        if("bot " != instruction.substr(0, 4))
            return false;

        istringstream iss{instruction};
        Bot_id bot_id;
        Relationship relationship;
        string word;

        iss >> word; //"bot"
        iss >> bot_id;

        iss >> word; //"gives"
        iss >> word; //"low"
        iss >> word; //"to"
        iss >> word; //"bot" or "output"
        relationship.low_is_output = (word == "output");
        iss >> relationship.low_id;

        iss >> word; //"and"
        iss >> word; //"high"
        iss >> word; //"to"
        iss >> word; //"bot" or "output"
        relationship.high_is_output = (word == "output");
        iss >> relationship.high_id;

        relationships[bot_id] = relationship;


        // keep track of max for bot_id and output_id
        max_bot_id = max(max_bot_id, bot_id);
        
        if(relationship.low_is_output)
            max_output_id = max(max_output_id, relationship.low_id);
        else
            max_bot_id = max(max_bot_id, relationship.low_id);

        if(relationship.high_is_output)
            max_output_id = max(max_output_id, relationship.high_id);
        else
            max_bot_id = max(max_bot_id, relationship.high_id);

        return true;
    }

    void process()
    {
        for(const auto p : placements)
        {
            bot_push(p.bot_id, p.value);
        }
    }

    void bot_push(Bot_id bot_id, Factory_value value)
    {
        Bot & bot_values = bots[bot_id];

        bot_values.push_back(value);

        if(bot_values.size() == bot_val_qty)
        {
            Factory_value low = min(bot_values[0], bot_values[1]);
            Factory_value high = max(bot_values[0], bot_values[1]);

            bot_values.clear();
            
            // track which value pairs are compared at each bot when they occur
            vals_compared_to_bot_id[{low, high}] = bot_id;

            if(relationships.end() == relationships.find(bot_id))
                throw logic_error("missing bot_id relationship");

            const auto & rel = relationships[bot_id];
            if(rel.low_is_output)
                outputs[rel.low_id].push_back(low);
            else
                bot_push(rel.low_id, low);

            if(rel.high_is_output)
                outputs[rel.high_id].push_back(high);
            else
                bot_push(rel.high_id, high);

        }
    }

    Bots bots;
    Outputs outputs;
    Placements placements;
    Relationships relationships;    
    Bot_id max_bot_id = 0;
    Output_id max_output_id = 0;
    map<pair<Factory_value, Factory_value>, size_t> vals_compared_to_bot_id;
    static constexpr size_t bot_val_qty{2};
};

Bot_id bot_id_that_compares_values(const Instructions & instructions, Factory_value val_a, Factory_value val_b)
{
    Timer t{"bot_id_that_compares_values"};
    Factory factory{instructions};
    return factory.bot_that_processes_values(val_a, val_b);
}

Factory_value mult_outputs(const Instructions & instructions, vector<Output_id> output_ids)
{
    Timer t{"mult_outputs"};
    Factory factory{instructions};

    Factory_value tally = 1;

    for(auto oid : output_ids)
    {
        tally *= factory.output_value(oid);
    }

    return tally;
}

int main()
{
    Timer t_main("main");

    Instructions instructions;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            instructions.push_back(line);
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    // auto result = bot_id_that_compares_values(instructions, 2, 5);
    // cout << "example 1 - bot id that compares 5 and 2: " << result << "\n";
    // cout << endl;

    auto result = bot_id_that_compares_values(instructions, 61, 17);
    cout << "part 1 - bot id that compares 61 and 17: " << result << "\n";
    cout << endl;

    auto result2 = mult_outputs(instructions, {0,1,2});
    cout << "part 2 - mult an output value from each output 0, 1, 2: " << result2 << "\n";
    cout << endl;
}
