#include <bits/stdc++.h>
using namespace std;

//a - 5255 too high

using Image = vector<string>;

string pixel_decoder;
Image image_org;

constexpr char p_on =  '#';
constexpr char p_off = '.';

char background = p_off;

constexpr size_t image_axis_exp = 2; //expands 1 unit in each direction

Image image_convert(const Image& in)
{
    size_t x_out_qty = in.size() + image_axis_exp;
    size_t y_out_qty = in[0].size() + image_axis_exp;

    //create a modified input, padded so that the following calcs do not overrun
    Image in_mod;
    in_mod.push_back(string(y_out_qty + image_axis_exp, background));
    in_mod.push_back(string(y_out_qty + image_axis_exp, background));
    for(auto r : in)
    {
        string row;
        row += background;
        row += background;
        row += r;
        row += background;
        row += background;
        in_mod.push_back( row );
    }
    in_mod.push_back(string(y_out_qty + image_axis_exp, background));
    in_mod.push_back(string(y_out_qty + image_axis_exp, background));

    Image out;
    for(size_t x = 0; x < x_out_qty; x++)
    {
        out.push_back("");
        for(size_t y = 0; y < y_out_qty; y++)
        {
            size_t decode_offset = 0; //9 bitwise

            size_t x_input = x + 1;
            size_t y_input = y + 1;

            // bit 8
            if(p_on == in_mod[x_input - 1][y_input - 1])  decode_offset |= 1UL << 8;
            if(p_on == in_mod[x_input - 1][y_input + 0])  decode_offset |= 1UL << 7;
            if(p_on == in_mod[x_input - 1][y_input + 1])  decode_offset |= 1UL << 6;
            if(p_on == in_mod[x_input + 0][y_input - 1])  decode_offset |= 1UL << 5;
            if(p_on == in_mod[x_input + 0][y_input + 0])  decode_offset |= 1UL << 4;
            if(p_on == in_mod[x_input + 0][y_input + 1])  decode_offset |= 1UL << 3;
            if(p_on == in_mod[x_input + 1][y_input - 1])  decode_offset |= 1UL << 2;
            if(p_on == in_mod[x_input + 1][y_input + 0])  decode_offset |= 1UL << 1;
            if(p_on == in_mod[x_input + 1][y_input + 1])  decode_offset |= 1UL << 0;

            out[x] += pixel_decoder[decode_offset];
        }
    }

    background = (background == p_on ? pixel_decoder[511] : pixel_decoder[0]);

    return out;
}

ostream& operator<<(ostream& os, const Image& image)
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
                parse_decoder = false;
            else if(parse_decoder)
                pixel_decoder += line;
            else
                image_org.push_back(line);
        }

        cout << "decoder:\n";

        cout << pixel_decoder << '\n';
        cout << '\n';
        cout << "original:\n";
        cout << image_org;
        cout << image_org.size() << " x " << image_org[0].size() << '\n';
        cout << '\n';

        vector<string> image_mod = image_org;
        background = p_off;
        for(size_t i = 0; i < 2; i++)
        {
            image_mod = image_convert(image_mod);

            cout << "convert out" << i+1 << ":\n";
            cout << image_mod;
            cout << image_mod.size() << " x " << image_mod[0].size() << '\n';
            cout << '\n';
        }
        unsigned long long count_on = 0;
        unsigned long long count_off = 0;
        for( auto row : image_mod )
            for(auto c : row)
                if(c == p_on)
                    count_on++;
                else if(c == p_off)
                    count_off++;
        cout << " a - light pixel count after 2 passes: " << count_on << "\n";

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
