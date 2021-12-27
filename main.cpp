#include <bits/stdc++.h>
using namespace std;

//a - 5255 too high

using Image = vector<string>;

string pixel_decoder;
Image image_org;

constexpr char p_on =  '#';
constexpr char p_off = '.';

constexpr size_t image_axis_exp = 2;


Image image_convert(const Image& in)
{
    Image out;
    int32_t x_out_qty = in.size() + image_axis_exp;
    int32_t y_out_qty = in[0].size() + image_axis_exp;

    int32_t x_in_min = 0;
    int32_t x_in_max = in.size() - 1;

    int32_t y_in_min = 0;
    int32_t y_in_max = in[0].size() - 1;

    for(int32_t x = 0; x < x_out_qty; x++)
    {
        out.push_back("");
        for(int32_t y = 0; y < y_out_qty; y++)
        {
            size_t decode_offset = 0; //9 bitwise

            int32_t x_input = x - 1;
            int32_t y_input = y - 1;

            // bit 8
            if( x_input >= x_in_min + 1 &&
                x_input <= x_in_max + 1 &&
                y_input >= y_in_min + 1 &&
                y_input <= y_in_max + 1 &&
                p_on == in[x_input - 1][y_input - 1] )
            {
                decode_offset |= 1UL << 8;
            }

            // bit 7
            if( x_input >= x_in_min + 1 &&
                x_input <= x_in_max + 1 &&
                y_input >= y_in_min + 0 &&
                y_input <= y_in_max + 0 &&
                p_on == in[x_input - 1][y_input] )
            {
                decode_offset |= 1UL << 7;
            }

            // bit 6
            if( x_input >= x_in_min + 1 &&
                x_input <= x_in_max + 1 &&
                y_input >= y_in_min - 1 &&
                y_input <= y_in_max - 1 &&
                p_on == in[x_input - 1][y_input + 1] )
            {
                decode_offset |= 1UL << 6;
            }

            // bit 5
            if( x_input >= x_in_min + 0 &&
                x_input <= x_in_max + 0 &&
                y_input >= y_in_min + 1 &&
                y_input <= y_in_max + 1 &&
                p_on == in[x_input][y_input - 1] )
            {
                decode_offset |= 1UL << 5;
            }

            // bit 4
            if( x_input >= x_in_min + 0 &&
                x_input <= x_in_max + 0 &&
                y_input >= y_in_min + 0 &&
                y_input <= y_in_max + 0 &&
                p_on == in[x_input][y_input] )
            {
                decode_offset |= 1UL << 4;
            }

            // bit 3
            if( x_input >= x_in_min + 0 &&
                x_input <= x_in_max + 0 &&
                y_input >= y_in_min - 1 &&
                y_input <= y_in_max - 1 &&
                p_on == in[x_input][y_input + 1] )
            {
                decode_offset |= 1UL << 3;
            }

            // bit 2
            if( x_input >= x_in_min - 1 &&
                x_input <= x_in_max - 1 &&
                y_input >= y_in_min + 1 &&
                y_input <= y_in_max + 1 &&
                p_on == in[x_input + 1][y_input - 1] )
            {
                decode_offset |= 1UL << 2;
            }

            // bit 1
            if( x_input >= x_in_min - 1 &&
                x_input <= x_in_max - 1 &&
                y_input >= y_in_min + 0 &&
                y_input <= y_in_max + 0 &&
                p_on == in[x_input + 1][y_input] )
            {
                decode_offset |= 1UL << 1;
            }

            // bit 0
            if( x_input >= x_in_min - 1 &&
                x_input <= x_in_max - 1 &&
                y_input >= y_in_min - 1 &&
                y_input <= y_in_max - 1 &&
                p_on == in[x_input + 1][y_input + 1] )
            {
                decode_offset |= 1UL << 0;
            }

            out[x] += pixel_decoder[decode_offset];
        }
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
            image_mod = image_convert(image_mod);

            cout << "convert out" << i+1 << ":\n";
            cout << image_mod;
            cout << '\n';
        }
        size_t count = 0;
        for( auto row : image_mod )
            for(auto c : row)
                if(c == p_on)
                    count++;
        cout << " a - pixel count after 2 passes: " << count << '\n';


        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
