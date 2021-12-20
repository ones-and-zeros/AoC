#include <bits/stdc++.h>
using namespace std;

#define TYPE_SUM 0
#define TYPE_PROD 1
#define TYPE_MIN 2
#define TYPE_MAX 3
#define TYPE_LITERAL 4
#define TYPE_GT 5
#define TYPE_LT 6
#define TYPE_EQ 7

size_t version_sum;

// result = {offset, value}
using result_t = pair<size_t,unsigned long>;

result_t parse_packet(string in) //expects binary string
{
    int version = stoi(in.substr(0,3), nullptr, 2);
    int type_id = stoi(in.substr(3,3), nullptr, 2);

    version_sum += version;

    size_t packet_length = 6;

    if(type_id == TYPE_LITERAL)
    {        
        string parse_s;
        while(1)
        {
            parse_s += in.substr(packet_length + 1, 4);
            packet_length += 5;
            if(in[packet_length-5] == '0')
                break;
        }
        // return literal packet
        return {packet_length, stoul(parse_s, nullptr, 2)};
    }

    // remaining are operator packets

    vector<unsigned long> packet_vals;

    if(in[6] == '0')
    {
        packet_length += 16;
        int bit_length = stoi(in.substr(7, 15), nullptr, 2);
        bit_length += packet_length;
        while(packet_length < bit_length)
        {
            result_t result = parse_packet(in.substr(packet_length));
            packet_length += result.first;
            packet_vals.push_back(result.second);
        }
    }
    else
    {
        packet_length += 12;
        int sub_packet_qty = stoi(in.substr(7, 11), nullptr, 2);
        size_t qty = 0;
        while(qty++ < sub_packet_qty)
        {
            result_t result = parse_packet(in.substr(packet_length));
            packet_length += result.first;
            packet_vals.push_back(result.second);
        }
    }

    unsigned long calced = 0;
    switch(type_id)
    {
        case TYPE_SUM:
        {
            for(auto n : packet_vals)
                calced += n;
            break; 
        }
        case TYPE_PROD:
        {
            calced = 1;
            for(auto n : packet_vals)
                calced *= n;
            break;
        }
        case TYPE_MIN:
        {
            calced = ~0UL;
            for(auto n : packet_vals)
                if(n < calced)
                    calced = n;
            break;
        }
        case TYPE_MAX:
        {
            calced = 0;
            for(auto n : packet_vals)
                if(n > calced)
                    calced = n;
            break;
        }
        case TYPE_GT:
        {
            if(packet_vals[0] > packet_vals[1])
                calced = 1;
            break;
        }
        case TYPE_LT:
        {
            if(packet_vals[0] < packet_vals[1])
                calced = 1;
            break;
        }
        case TYPE_EQ:
        {
            if(packet_vals[0] == packet_vals[1])
                calced = 1;
            break;
        }
    }

    return {packet_length, calced};
}

string hex_to_bin(const string& hex)
{
    string bin;

    for(auto c : hex)
    {
        switch(c)
        {
            case '0':
                bin += "0000";
                break;
            case '1':
                bin += "0001";
                break;
            case '2':
                bin += "0010";
                break;
            case '3':
                bin += "0011";
                break;
            case '4':
                bin += "0100";
                break;
            case '5':
                bin += "0101";
                break;
            case '6':
                bin += "0110";
                break;
            case '7':
                bin += "0111";
                break;
            case '8':
                bin += "1000";
                break;
            case '9':
                bin += "1001";
                break;
            case 'A':
                bin += "1010";
                break;
            case 'B':
                bin += "1011";
                break;
            case 'C':
                bin += "1100";
                break;
            case 'D':
                bin += "1101";
                break;
            case 'E':
                bin += "1110";
                break;
            case 'F':
                bin += "1111";
                break;
        }
    }

    return bin;
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    vector<string> input;

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        string input_s;

        //parse lines
        while(getline(infile, file_line))
        {
            // istringstream iss{file_line};
            // string key, val, junk;
            // getline(iss, key, ' ');

            input.push_back(file_line);
        }

        for(auto row : input)
        {
            version_sum = 0;
            result_t result = parse_packet(hex_to_bin(row));
            cout << row << endl;
            cout << "a - version sum: " << version_sum << endl;
            cout << "b - calculated: " << result.second << endl;
        }

        cout << endl;
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
