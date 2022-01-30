#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

struct Command{
    Command(string raw)
    {
        if(raw.substr(0,4) == "rect")
        {
            opcode = Opcode::rect;
            istringstream iss{raw.substr(4, raw.size() - 4)};
            char the_x;
            iss >> a;
            iss >> the_x;
            iss >> b;
            return;
        }
        if(raw.substr(0,13) == "rotate row y=")
        {
            opcode = Opcode::rrow;
            istringstream iss{raw.substr(13, raw.size() - 13)};
            string word_by;
            iss >> a;
            iss >> word_by;
            iss >> b;
            return;
        }
        if(raw.substr(0,16) == "rotate column x=")
        {
            opcode = Opcode::rcol;
            istringstream iss{raw.substr(16, raw.size() - 16)};
            string word_by;
            iss >> a;
            iss >> word_by;
            iss >> b;
            return;
        }
        throw logic_error("invalid opcode parse");
    }
    enum Opcode{rect, rcol, rrow};
    Opcode opcode;
    size_t a = 0;
    size_t b = 0;
};

ostream & operator<<(ostream & os, Command cmd)
{
    if(cmd.opcode == Command::Opcode::rect)
        os << "rect ";
    else if(cmd.opcode == Command::Opcode::rcol)
        os << "rcol ";
    else if(cmd.opcode == Command::Opcode::rrow)
        os << "rrow ";

    os << cmd.a << " " << cmd.b << "\n";

    return os;
}

using Commands = vector<Command>;

void string_rrot(string & s, size_t qty)
{
    string temp = s.substr((s.size() - qty), qty) + s.substr(0, s.size() - qty);
    s = temp;
}

class Little_screen{
public:
    Little_screen(Commands commands)
    {
        // cout << "start:\n";
        // cout << *this << endl;

        for(auto c : commands)
        {
            if(c.opcode == Command::Opcode::rect)
                rect(c.a, c.b);
            else if(c.opcode == Command::Opcode::rcol)
                rcol(c.a, c.b);
            else if(c.opcode == Command::Opcode::rrow)
                rrow(c.a, c.b);

            // cout << c;
            // cout << *this << endl;
        }
    }

    uint32_t pixel_on_qty() const
    {
        uint32_t tally = 0;
        for(auto row : screen)
            tally += count(row.begin(), row.end(), pixel_on);
        return tally;
    }

    friend ostream & operator<<(ostream & os, const Little_screen & ls);

private:
    void rect(size_t width, size_t height)
    {
        for(size_t h = 0; h < height; ++h)
            screen[h].replace(0, width, string(width, {this->pixel_on}));
    }

    void rrow(uint32_t row, size_t qty)
    {
        if(row >= screen_height)
            throw logic_error("invalid row");

        string_rrot(screen[row], qty);
    }

    void rcol(uint32_t column, size_t qty)
    {
        if(column >= screen_width)
            throw logic_error("invalid column");

        if(column == 3 && qty == 5)
        {
            asm("nop");
        }

        string temp;
        for(auto line : screen)
            temp += line[column];

        string_rrot(temp, qty);

        for(size_t pos = 0; pos < screen_height; pos++)
            screen[pos][column] = temp[pos];
    }

    static constexpr size_t screen_height{6};
    static constexpr size_t screen_width{50};
    static constexpr char pixel_off{'.'};
    static constexpr char pixel_on{'#'};
    vector<string> screen{ screen_height, {string(screen_width, pixel_off)}};
};
// declarations outside of class required for linker
constexpr char Little_screen::pixel_on;

ostream & operator<<(ostream & os, const Little_screen & ls)
{
    for(const auto row : ls.screen)
        os << row << "\n";
    return os;
}

uint32_t qty_of_pixels_on(Commands commands)
{
    Timer t{"qty_pixels"};
    Little_screen screen(commands);
    cout << screen;
    return screen.pixel_on_qty();
}

int main()
{
    Timer t_main("main");

    Commands commands; 

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            commands.push_back(line);
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto result = qty_of_pixels_on(commands);
    cout << "part 1 - qty pixels on: " << result << "\n";
    cout << "part 2 - read rendered screen txt\n";
    cout << endl;
}
