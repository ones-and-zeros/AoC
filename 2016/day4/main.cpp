#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

class Room{
public:
    Room(string name, string sector_id, string checksum)
    : name_{name}, sector_id_{sector_id}, checksum_{checksum}
    {
    }
    bool is_valid() const
    {
        return checksum_ == gen_checksome();
    }
    int32_t sector_id_val() const
    {
        return stol(sector_id_);
    }
private:
    struct Letter_count{
        Letter_count(char letter, int32_t count)
        : letter_{letter}, count_{count}
        {
        }
        char letter_;
        int32_t count_;

        // for priority_queue using 'std::less'
        bool operator<(const Letter_count &other)
        {
            if(this->count_ != other.count_)
                return (this->count_ > other.count_);
            return (this->letter_ < other.letter_);
        }
    };

    string gen_checksome() const
    {
        // count letters
        array<int32_t,26> letters{0};
        for(auto c : name_)
        {
            if(isalpha(c))
            {
                letters[c - 'a']++;
            }
        }

        vector<Letter_count> letter_count;
        for(char letter = 'a'; letter <= 'z'; letter++)
        {
            letter_count.push_back({letter, letters[letter - 'a']});
        }
        partial_sort(letter_count.begin(), letter_count.begin() + 5, letter_count.end());
        
        string result;
        for(size_t pos = 0; pos < 5; pos++)
        {
            result += letter_count[pos].letter_;
        }

        return result;
    }
    string name_;
    string sector_id_;
    string checksum_;
};

using Rooms = vector<Room>;


int32_t valid_room_sum(Rooms rooms)
{
    Timer t{"valid_room_sum"};
    int32_t tally = 0;

    for(auto r : rooms)
    {
        if(r.is_valid())
        {
            tally += r.sector_id_val();
        }
    }
    return tally;
}

int main()
{
    Timer t_main("main");

    Rooms rooms;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            string name;
            string vector_id;
            string checksum;

            size_t pos = 0;
            while(!isdigit(line[pos + 1]))
            {
                name += line[pos++];
            }
            pos++;
            while(line[pos] != '[')
            {
                vector_id += line[pos++];
            }
            pos++;
            while(line[pos] != ']')
            {
                checksum += line[pos++];
            }
            rooms.push_back({name, vector_id, checksum});
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto result = valid_room_sum(rooms);
    cout << "part 1 - valid room sum: " << result << "\n";
    cout << "\n";

    // for(auto r : rooms)
    // {
    //     cout << r.name_ << "-" << stol(r.sector_id_) << "[" << r.checksum_ << "]\n";
    // }

}
