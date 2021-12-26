#include <bits/stdc++.h>
using namespace std;

using Image = vector<string>;

string pixel_decoder;
Image image_org;

constexpr char p_on =  '#';
constexpr char p_off = '.';

Image image_convert(Image& in)
{
    Image out;

    // dummy - just invert for now
    for(auto r : in)
    {
        string r_out;
        for(auto c : r)
            r_out += ((c == p_off) ? p_on : p_off);
        out.push_back(r_out);
    }

    return out;
}

ostream& operator<<(ostream& os, Image image)
{
    for(auto row : image)
        os << row << '\n';
    return os;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        bool parse_decoder = true;
        //parse lines
        while(getline(infile, line))
        {
            if(line.empty())
            {
                parse_decoder = false;
                continue;
            }

            if(parse_decoder)
            {
                pixel_decoder += line;
                continue;
            }

            image_org.push_back(line);
        }

        cout << "decoder:\n";

        cout << pixel_decoder << '\n';
        cout << '\n';
        cout << "original:\n";
        cout << image_org;
        cout << '\n';


        vector<string> image_mod = image_org;
        for(size_t i = 0; i < 2; i++)
        {
            cout << "convert in" << i+1 << ":\n";
            cout << image_mod;
            image_mod = image_convert(image_mod);

            cout << "convert out" << i+1 << ":\n";
            cout << image_mod;
            cout << '\n';
        }

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
