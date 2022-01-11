#include <bits/stdc++.h>
using namespace std;

// #############
// #01.2.3.4.56#
// ###0#1#2#3###
//   #A#B#C#D#
//   #########

//#define LOG_MOVES

constexpr size_t room_qty{4};
constexpr size_t hall_length{7}; //only available spots
constexpr size_t room_depth{2};
constexpr size_t room_top{0};
constexpr size_t room_bottom{1};
constexpr char unoccupied{'.'};

struct Board{
    Board( vector<string> init )
      : rooms{init}
    {
        if(init.size() != room_qty)
            throw range_error("invalid room qty");
        for(auto r : init)
            if(r.size() != room_depth)
                throw range_error("invalid room size");
    }
    string hall{ string(hall_length, '.') };
    vector<string> rooms;
};

// #############
// #01.2.3.4.56#
// ###0#1#2#3###
//   #A#B#C#D#
//   #########
ostream& operator<<(ostream& os, Board& b)
{
    os << "#############\n";
    os << "#" << b.hall[0] << b.hall[1];
    os << "." << b.hall[2];
    os << "." << b.hall[3];
    os << "." << b.hall[4];
    os << "." << b.hall[5] << b.hall[6] << "#\n";
    os << "###" << b.rooms[0][room_top];
    os << "#" << b.rooms[1][room_top];
    os << "#" << b.rooms[2][room_top];
    os << "#" << b.rooms[3][room_top] << "###\n";
    os << "  #" << b.rooms[0][room_bottom];
    os << "#" << b.rooms[1][room_bottom];
    os << "#" << b.rooms[2][room_bottom];
    os << "#" << b.rooms[3][room_bottom] << "#\n";
    os << "  #########\n";

    return os;
}

uint64_t energy = 0;
uint64_t least_energy = ~0;
uint64_t win_count = 0;
#ifdef LOG_MOVES
vector<string> move_log;
vector<Board> move_board;
#endif

// {room_idx, hall_idx} -> { move_count, hall_blocking }
map<pair<size_t, size_t>, pair<int, vector<size_t>>> room_to_hall{
    {{0, 0}, {3, {1}}},
    {{0, 1}, {2, { }}},
    {{0, 2}, {2, { }}},
    {{0, 3}, {4, {2}}},
    {{0, 4}, {6, {2,3}}},
    {{0, 5}, {8, {2,3,4}}},
    {{0, 6}, {9, {2,3,4,5}}},

    {{1, 0}, {5, {1,2}}},
    {{1, 1}, {4, {2}}},
    {{1, 2}, {2, { }}},
    {{1, 3}, {2, { }}},
    {{1, 4}, {4, {3}}},
    {{1, 5}, {6, {3,4}}},
    {{1, 6}, {7, {3,4,5}}},

    {{2, 0}, {7, {1,2,3}}},
    {{2, 1}, {6, {2,3}}},
    {{2, 2}, {4, {3}}},
    {{2, 3}, {2, { }}},
    {{2, 4}, {2, { }}},
    {{2, 5}, {4, {4}}},
    {{2, 6}, {5, {4,5}}},

    {{3, 0}, {9, {1,2,3,4}}},
    {{3, 1}, {8, {2,3,4}}},
    {{3, 2}, {6, {3,4}}},
    {{3, 3}, {4, {4}}},
    {{3, 4}, {2, { }}},
    {{3, 5}, {2, { }}},
    {{3, 6}, {3, {5}}}
};

// {room_idx, room_idx} -> { move_count, hall_blocking }
map<pair<size_t, size_t>, pair<int, vector<size_t>>> room_to_room{
    {{0, 1}, {4, {2}}},
    {{0, 2}, {6, {2,3}}},
    {{0, 3}, {8, {2,3,4}}},

    {{1, 0}, {4, {2}}},
    {{1, 2}, {4, {3}}},
    {{1, 3}, {6, {3,4}}},

    {{2, 0}, {6, {2,3}}},
    {{2, 1}, {4, {3}}},
    {{2, 3}, {4, {4}}},

    {{3, 0}, {8, {2,3,4}}},
    {{3, 1}, {6, {3,4}}},
    {{3, 2}, {4, {4}}},
};

bool is_blocked_rh(Board& board, size_t room_idx, size_t hall_idx)
{
    auto intermediate = room_to_hall[{room_idx, hall_idx}].second;
    for(auto h : intermediate)
        if(board.hall[h] != unoccupied)
            return true;
    return false;
}

bool is_blocked_rr(Board& board, size_t room_s_idx, size_t room_t_idx)
{
    auto intermediate = room_to_room[{room_s_idx, room_t_idx}].second;
    for(auto h : intermediate)
        if(board.hall[h] != unoccupied)
            return true;
    return false;
}

map<char, uint64_t> piece_energy { {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000} };

bool is_win(Board& board)
{
    for(size_t r = 0; r < board.rooms.size(); r++)
        if(board.rooms[r][0] != ('A' + r))
            return false;
    return true;
}

void next_move(Board& board)
{
    // cout << board;
    // cout << '\n';
    
    if(energy >= least_energy)
        return;

    //does it win?
    if(is_win(board))
    {
#ifdef LOG_MOVES
        // cout << "win!!! " << energy << "\n";
        // for(size_t i = 0; i < move_log.size(); i++)
        // {
        //     cout << move_log[i] << '\n';
        //     cout << move_board[i];  //already \n included
        // }
        // cout << '\n';
        // cout << '\n';
#endif

        win_count++;
        if(energy < least_energy)
            least_energy = energy;
        return;
    }

    // can any piece in hallway go home?
    for(size_t h = 0; h < hall_length; h++)
    {
        // identify if piece is present
        char piece = board.hall[h];
        if(piece == unoccupied)
            continue;

        // find target room            
        size_t t = piece - 'A';
        // see if room is avail
        size_t t_pos = 0;
        if( board.rooms[t][room_top] != unoccupied )
            continue;
        else if( board.rooms[t][room_bottom] == unoccupied )
            t_pos = room_bottom;
        else if(board.rooms[t][room_bottom] == piece)
            t_pos = room_top;
        else
            continue;

        // ensure it is not blocked
        if(is_blocked_rh(board, t, h))
            continue;

        //move h -> tr
        board.rooms[t][t_pos] = piece;
        board.hall[h] = unoccupied;

        uint64_t this_energy = (room_to_hall[{t, h}].first + t_pos) * piece_energy[piece];
        energy += this_energy;

#ifdef LOG_MOVES
        move_log.push_back(string(1, piece) + " h" + to_string(h) + " -> r" + to_string(t) + ", e +" + to_string(this_energy));
        move_board.push_back(board);
#endif
        next_move(board);

        //undo move and try next
        energy -= this_energy;
        board.rooms[t][t_pos] = unoccupied;
        board.hall[h] = piece;

#ifdef LOG_MOVES
        move_log.pop_back();
        move_board.pop_back();
#endif
    }

    // can any piece in wrong room go home?
    for(size_t s = 0; s < room_qty; s++)
    {
        size_t s_pos = 0;
        if(board.rooms[s][room_top] != ('A' + s) &&
           board.rooms[s][room_top] != unoccupied)
        {
            s_pos = room_top;
        }
        else if(board.rooms[s][room_top] == unoccupied &&
                board.rooms[s][room_bottom] != unoccupied &&
                board.rooms[s][room_bottom] != ('A' + s))
        {
            s_pos = room_bottom;
        }
        else
            continue;

        // identify piece;
        char piece = board.rooms[s][s_pos];

        // find target room            
        size_t t = piece - 'A';
        // see if room is avail
        size_t t_pos = 0;
        if( board.rooms[t][room_top] != unoccupied )
            continue;
        else if( board.rooms[t][room_bottom] == unoccupied )
            t_pos = room_bottom;
        else if(board.rooms[t][room_bottom] == piece)
            t_pos = room_top;
        else
            continue;

        // ensure it is not blocked
        if(is_blocked_rr(board, s, t))
            continue;

        //move s -> tr
        board.rooms[t][t_pos] = piece;
        board.rooms[s][s_pos] = unoccupied;

        uint64_t this_energy = (room_to_room[{s, t}].first + s_pos + t_pos) * piece_energy[piece];
        energy += this_energy;

#ifdef LOG_MOVES
        move_log.push_back(string(1, piece) + " r" + to_string(s) + " -> r" + to_string(t) + ", e +" + to_string(this_energy));
        move_board.push_back(board);
#endif

        next_move(board);

        //undo move and try next
        energy -= this_energy;
        board.rooms[t][t_pos] = unoccupied;
        board.rooms[s][s_pos] = piece;

#ifdef LOG_MOVES
        move_log.pop_back();
        move_board.pop_back();
#endif
    }

    // can any piece move out of room?
    for(size_t s = 0; s < room_qty; s++)
    {
        size_t s_pos = 0;
        char home = ('A' + s);
        if(board.rooms[s][room_bottom] != home &&
           board.rooms[s][room_top] != unoccupied)
        {
            s_pos = room_top;
        }
        else if(board.rooms[s][room_bottom] == home &&
                board.rooms[s][room_top] != unoccupied &&
                board.rooms[s][room_top] != home)
        {
            s_pos = room_top;
        }
        else if(board.rooms[s][room_top] == unoccupied &&
                board.rooms[s][room_bottom] != unoccupied &&
                board.rooms[s][room_bottom] != home)
        {
            s_pos = room_bottom;
        }
        else
            continue;

        // identify piece;
        char piece = board.rooms[s][s_pos];

        for(size_t h = 0; h < hall_length; h++)
        {
            if(board.hall[h] != unoccupied)
                continue;

            // ensure it is not blocked
            if(is_blocked_rh(board, s, h))
                continue;

            //move s -> h
            board.hall[h] = piece;
            board.rooms[s][s_pos] = unoccupied;

            uint64_t this_energy = (room_to_hall[{s, h}].first + s_pos) * piece_energy[piece];
            energy += this_energy;

#ifdef LOG_MOVES
            move_log.push_back(string(1, piece) + " r" + to_string(s) + " -> h" + to_string(h) + ", e +" + to_string(this_energy));
            move_board.push_back(board);
#endif
            next_move(board);

            //undo move and try next
            energy -= this_energy;
            board.hall[h] = unoccupied;
            board.rooms[s][s_pos] = piece;

#ifdef LOG_MOVES
            move_log.pop_back();
            move_board.pop_back();
#endif

        }
    }
}

uint64_t play(vector<string> init)
{
    Board board(init);
    energy = 0;
    least_energy = ~0;
    win_count = 0;
    
    next_move(board);
    
    return least_energy;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> room_init{room_qty, string("")};

        //parse lines
        while(getline(infile, line))
        {
            if(isalpha(line[3]))
            {
                room_init[0] += line[3];
                room_init[1] += line[5];
                room_init[2] += line[7];
                room_init[3] += line[9];
            }
        }

        play(room_init);
        cout << "     wins: " << win_count << '\n';
        cout << " a - least power: " << least_energy << '\n';

        cout << '\n';
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
