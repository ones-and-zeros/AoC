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
    string name() const
    {
        return name_;
    }
    string decoded_name() const
    {
        size_t offset = sector_id_val() % 26;

        string result;
        for(auto c : name_)
        {
            if(c == '-')
            {
                result += ' ';
                continue;
            }
            if(c + offset <= 'z')
                result += c + offset;
            else
                result += c - (26 - offset);
        }
        return result;
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
                letters[c - 'a']++;
        }

        vector<Letter_count> letter_count;
        for(char letter = 'a'; letter <= 'z'; letter++)
            letter_count.push_back({letter, letters[letter - 'a']});
        partial_sort(letter_count.begin(), letter_count.begin() + 5, letter_count.end());
        
        string result;
        for(size_t pos = 0; pos < 5; pos++)
            result += letter_count[pos].letter_;

        return result;
    }
    string name_;
    string sector_id_;
    string checksum_;
};

using Rooms = vector<Room>;


int32_t valid_room_sum(const Rooms &rooms)
{
    Timer t{"valid_room_sum"};
    int32_t tally = 0;

    for(auto &r : rooms)
        if(r.is_valid())
            tally += r.sector_id_val();

    return tally;
}

int32_t northpole_object_storage_vector_id(const Rooms &rooms)
{
    Timer t{"decoded_names"};

    vector<string> result;

    for(auto &r : rooms)
        if(r.is_valid())
            if("northpole object storage" == r.decoded_name())
                return r.sector_id_val();
    return -1;
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
                name += line[pos++];
            pos++;
            while(line[pos] != '[')
                vector_id += line[pos++];
            pos++;
            while(line[pos] != ']')
                checksum += line[pos++];
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

    result = northpole_object_storage_vector_id(rooms);
    cout << "part 2 - decoded names: " << result << "\n";
    cout << "\n";
}
