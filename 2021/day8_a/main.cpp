#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>


// 0 a b c . e f g   6
// 1 . . c . . f .   2*
// 2 a . c d e . g   5
// 3 a . c d . f g   5
// 4 . b c d . f .   4*
// 5 a b . d . f g   5
// 6 a b . d e f g   6
// 7 a . c . . f .   3*
// 8 a b c d e f g   7*
// 9 a b c d . f g   6
//
//   8 6 8 7 4 9 7
//   a b c d e f g
//     *     * *

//step 1 - identify unique vals
// end:
// segs:
// b = 6
// e = 4
// f = 9
// pattern:
// 1 = 2
// 4 = 4
// 7 = 3
// 8 = 7

//step 2 - calc remaining segments
// a = 7 - 1
// c = 1 - f
// d = 4 - bcf
// g = 8 - abcdef

using Display = std::string;
using Cypher = std::map<char,char>;

const std::array<Display,10> display_lookup = {
    "abcefg",   // 0 a b c . e f g
    "cf",       // 1 . . c . . f .
    "acdeg",    // 2 a . c d e . g
    "acdfg",    // 3 a . c d . f g
    "bcdf",     // 4 . b c d . f .
    "abdfg",    // 5 a b . d . f g
    "abdefg",   // 6 a b . d e f g
    "acf",      // 7 a . c . . f .
    "abcdefg",  // 8 a b c d e f g
    "abcdfg"    // 9 a b c d . f g
};

// tally of each segment for all 10 digis
constexpr std::array<size_t,7>  seg_tally_all{8,6,8,7,4,9,7};
                                           // a b c d e f g
//calc index using char 'a'= 0 through 'g' = 6
size_t seg_tally_index(char c)
{
    if(c >= 'a' && c <= 'g')
        return c - 'a';        
    return 0;
}

// identify unique char
char disp_diff(const Display val, const Display to_remove )
{
    Display out{val};
    for(auto c : to_remove)
        out.erase( std::remove(out.begin(), out.end(), c), out.end());
    return out[0];
}

constexpr size_t panel_pattern_qty = 10;
constexpr size_t panel_digit_qty = 4;
struct Panel {
    Panel()
    {
    }
    Display patterns[panel_pattern_qty];
    Display digits[panel_digit_qty];
    Cypher cypher;
    friend std::ostream& operator<<(std::ostream& os, const Panel& p)
    {
        for(size_t i = 0; i < panel_pattern_qty; i++)
            os << p.patterns[i] << " ";
        os << "- ";
        for(size_t i = 0; i < panel_digit_qty; i++)
            os << p.digits[i] << " ";
        return os;
    }
};

int decode_digit(Display in, Cypher& cyph)
{
    Display disp;
    for(auto c : in)
        disp += cyph[c];
    std::sort(disp.begin(), disp.end());
    for(int d = 0; d < 10; d++)
        if(display_lookup[d] == disp)
            return d;
    return 0;
}


int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::vector<Panel> panels;

        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
        {
            std::istringstream iss(std::move(file_line));
            std::string s;

            Panel panel;

            for(size_t p = 0; p < panel_pattern_qty; p++)
            {
                std::getline(iss, s, ' ');
                std::sort(s.begin(), s.end());
                panel.patterns[p] = Display{s};
            }
            std::getline(iss, s, ' '); //throw away '|'
            for(size_t d = 0; d < panel_digit_qty; d++)
            {
                std::getline(iss, s, ' ');
                std::sort(s.begin(), s.end());
                panel.digits[d] = Display{s};
            }

            panels.push_back(panel);
        }


        // for(auto pan : panels)
        // {
        //     auto digit_1 = std::find(panel.digits.begin();)
        //     Display
        //     Display& 
        // }

        // for(auto& p : panels)
        //     std::cout << p << " " << std::endl;
        // std::cout << std::endl;

        for(auto& pan : panels)
        {
            std::map<size_t,Display> dig_to_disp;
            Cypher cyph_reg_to_scram;
            Cypher cyph_decode;

            //find unique sized digits first
            //1,4,7,8
            for(auto pat : pan.patterns)
            {
                if(pat.size() == display_lookup[1].size())
                    dig_to_disp.insert({1,pat});
                if(pat.size() == display_lookup[4].size())
                    dig_to_disp.insert({4,pat});
                if(pat.size() == display_lookup[7].size())
                    dig_to_disp.insert({7,pat});
                if(pat.size() == display_lookup[8].size())
                    dig_to_disp.insert({8,pat});
            }

            //find unique segment counts
            //b,e,f
            for(char c = 'a'; c <= 'g'; c++)
            {
                size_t count = 0;
                for(auto pan : pan.patterns)
                    count += std::count(pan.begin(), pan.end(), c);

                // check for known counts
                if(count == seg_tally_all[seg_tally_index('b')])
                    cyph_reg_to_scram.insert({'b', c});
                if(count == seg_tally_all[seg_tally_index('e')])
                    cyph_reg_to_scram.insert({'e', c});
                if(count == seg_tally_all[seg_tally_index('f')])
                    cyph_reg_to_scram.insert({'f', c});
            }

            // a = 7 - 1
            Display to_remove;
            to_remove = dig_to_disp[1];
            cyph_reg_to_scram.insert({ 'a', disp_diff(dig_to_disp[7], to_remove)});

            // c = 1 - f
            to_remove.clear();
            to_remove += cyph_reg_to_scram['f'];
            cyph_reg_to_scram.insert({ 'c', disp_diff(dig_to_disp[1], to_remove)});

            // d = 4 - bcf
            to_remove.clear();
            to_remove += cyph_reg_to_scram['b'];
            to_remove += cyph_reg_to_scram['c'];
            to_remove += cyph_reg_to_scram['f'];
            cyph_reg_to_scram.insert({ 'd', disp_diff(dig_to_disp[4], to_remove)});

            // g = 8 - abcdef
            to_remove.clear();
            to_remove += cyph_reg_to_scram['a'];
            to_remove += cyph_reg_to_scram['b'];
            to_remove += cyph_reg_to_scram['c'];
            to_remove += cyph_reg_to_scram['d'];
            to_remove += cyph_reg_to_scram['e'];
            to_remove += cyph_reg_to_scram['f'];
            cyph_reg_to_scram.insert({ 'g', disp_diff(dig_to_disp[8], to_remove)});
            
            // static int pass = 0;
            // std::cout << "panel " << ++pass << ":" << std::endl;
            // std::cout << "reg -> scram:" << std::endl;
            // for(char c = 'a'; c <= 'g'; c++)
            // {
            //     std::cout << c << "=";
            //     if(cyph_reg_to_scram.end() != cyph_reg_to_scram.find(c))
            //         std::cout << cyph_reg_to_scram[c];
            //     std::cout << std::endl;
            // }

            //create new cypher to decode
            for(auto rs : cyph_reg_to_scram)
                cyph_decode.insert({rs.second, rs.first});


            // std::cout << "scram -> reg:" << std::endl;
            // for(char c = 'a'; c <= 'g'; c++)
            // {
            //     std::cout << c << "=";
            //     if(cyph_decode.end() != cyph_decode.find(c))
            //         std::cout << cyph_decode[c];
            //     std::cout << std::endl;
            // }
            // std::cout << std::endl;

            pan.cypher = cyph_decode;
        }
        //deafgbc
        //abcdefg

        size_t count_1478 = 0;
        for(auto& pan : panels)
        {
            for(auto& dig : pan.digits)
            {
                if(dig.size() == display_lookup[1].size())
                    count_1478++;
                if(dig.size() == display_lookup[4].size())
                    count_1478++;
                if(dig.size() == display_lookup[7].size())
                    count_1478++;
                if(dig.size() == display_lookup[8].size())
                    count_1478++;
            }
        }
        std::cout << "out dig count 1,4,7,8: " << count_1478 << std::endl;

        long int total = 0;
        for(auto& pan : panels)
        {
            int val = 0;
            int weight = 1000;
            for(auto& dig : pan.digits)
            {
                val += decode_digit(dig, pan.cypher) * weight;
                weight /= 10;
            }
            total += val;
            std::cout << val << std::endl;
        }
        std::cout << std::endl;
        std::cout << "total: " << total << std::endl;

        std::cout << std::endl;
        infile.close();
   }
}


