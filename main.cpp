#include <bits/stdc++.h>
using namespace std;

#define TYPE_LITERAL 4

size_t version_sum;

//#define DEBUG_PRINTS

size_t parse_packet(string in) //expects binary string
{
#ifdef DEBUG_PRINTS
    cout << in << endl;
    cout << "vvvttt";
#endif

    int version = stoi(in.substr(0,3), nullptr, 2);
    int type_id = stoi(in.substr(3,3), nullptr, 2);
    size_t packet_length = 6;

    string parse_s;
    if(type_id == TYPE_LITERAL)
    {        
        for(int offset = 6; offset < in.size(); offset += 5)
        {
            packet_length += 5;
            parse_s += in.substr(offset + 1, 4);
#ifdef DEBUG_PRINTS
            cout << " LLLL";
#endif
            if(in[offset] == '0')
                break;
        }
#ifdef DEBUG_PRINTS
        cout << endl;
        cout << " packet literal: " << stoi(parse_s, nullptr, 2) << endl;
#endif
    }
    else
    {
        // operator packet
        if(in[6] == '0')
        {
#ifdef DEBUG_PRINTS
            cout << "B123456789012345" << endl;
#endif
            int bit_length = stoi(in.substr(7, 15), nullptr, 2);
            packet_length += 16;
            //remaining bits account for sub packets
            size_t read = 0;
            while(read < bit_length)
            {
#ifdef DEBUG_PRINTS
                cout << " sub parse bits:  " << read << " of " << bit_length << endl;
#endif
                read += parse_packet(in.substr(22 + read));
            }
            packet_length += read;
        }
        else
        {
#ifdef DEBUG_PRINTS
            cout << "P12345678901" << endl;
#endif
            int sub_packet_qty = stoi(in.substr(7, 11), nullptr, 2);
            packet_length += 12;
            //remaining bits account for sub packets
            size_t read = 0;
            size_t qty = 0;
            while(qty < sub_packet_qty)
            {
#ifdef DEBUG_PRINTS
                cout << " sub parse p:  " << qty << "(" << read << ") of " << sub_packet_qty << endl;
#endif
                read += parse_packet(in.substr(18 + read));
                qty++;
            }
            packet_length += read;
        }
    }

    version_sum += version;

    return packet_length;
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
            cout << row << endl;
        cout << endl;

        for(auto row : input)
        {
            version_sum = 0;
            parse_packet(hex_to_bin(row));
            cout << "a - version sum: " << version_sum << endl;
            cout << endl;
            cout << endl;
        }
        cout << endl; 

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
