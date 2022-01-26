#include <bits/stdc++.h>
using namespace std;

// #############
// #01.2.3.4.56#
// ###A#B#C#D###
//   #1#1#1#1#
//   #2#2#2#2#
//   #3#3#3#3#
//   #########

// priority
//   for(d .. a)
//     clear
//     fill


//


// brainstorm

// - identify a blocking move and try to
//   regress back to that move and change it

// - go greedy and back off as required

// - moves in\out of base are fixed moves,
//   unique and constant per individual game start layout
//   these are the direct moves out/in of rooms with no intermidate point
//   every winning game played uses *at least* these moves
//   and everything is a delta against this baseline
//   so, calc potential for remaiming current game and exit early.
//   could also potentially simplify end game calcs


// Plan 2.0

// init
// 1. flag amphipods that are already in correct room,
//    identified as not blocking any other types from escaping
// 2. calculate base score, as if blocking did not matter

// runtime
// 1. cycle, then repeat from top:
//   a) move piece home, from hall; priority D to A
//   b) move piece home, from room; priority D to A
//   c) move piece from room to hall
//          priority 1) anything blocking of D to A
//          priority 2) move least amount of steps out of typical direction
//   d) if blocked, identity blocking piece and rollback the move to retry from there
// 2. calculate any moves outside of baseline moves, count as "delta score", 
//    and **end** if that exceeds prior best
// 3. win score = base + delta










//#define LOG_MOVES

//#define SOLUTION_B

#ifdef SOLUTION_B
constexpr size_t room_depth{4};
#else
constexpr size_t room_depth{2};
#endif

constexpr size_t room_qty{4};
constexpr size_t hall_length{7}; //only available spots
constexpr size_t room_top{0};
constexpr size_t room_bottom{room_depth-1};
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
// ###A#B#C#D###
//   #1#1#1#1#
//   #2#2#2#2#
//   #3#3#3#3#
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
    os <<   "#" << b.rooms[1][room_top];
    os <<   "#" << b.rooms[2][room_top];
    os <<   "#" << b.rooms[3][room_top] << "###\n";
    for(size_t i = 1; i <= room_bottom; i++)
    {
        os << "  #" << b.rooms[0][room_bottom] <<
                "#" << b.rooms[1][room_bottom] <<
                "#" << b.rooms[2][room_bottom] <<
                "#" << b.rooms[3][room_bottom] << "#\n";
    }
    os << "  #########\n";

    return os;
}

uint64_t win_count = 0;
vector<bool> room_avail(room_qty, false);
vector<size_t> room_pos(room_qty, room_top);

Board game_board{
  {{room_depth, {'A'}}, 
   {room_depth, {'B'}}, 
   {room_depth, {'C'}}, 
   {room_depth, {'D'}}}
};
uint64_t base_energy = 0;
uint64_t delta_energy = 0;
uint64_t least_delta_energy_win = 0;


#ifdef LOG_MOVES
vector<string> move_log;
vector<Board> move_board;
#endif

map<char, uint64_t> piece_energy { {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000} };

// {room_idx, hall_idx} -> { move_count, hall_blocking }
map<pair<size_t, size_t>, pair<int, vector<size_t>>> room_to_hall{
    {{0, 0}, {2, {1}}},
    {{0, 1}, {1, { }}},
    {{0, 2}, {1, { }}},
    {{0, 3}, {3, {2}}},
    {{0, 4}, {5, {2,3}}},
    {{0, 5}, {7, {2,3,4}}},
    {{0, 6}, {8, {2,3,4,5}}},

    {{1, 0}, {4, {1,2}}},
    {{1, 1}, {3, {2}}},
    {{1, 2}, {1, { }}},
    {{1, 3}, {1, { }}},
    {{1, 4}, {3, {3}}},
    {{1, 5}, {5, {3,4}}},
    {{1, 6}, {6, {3,4,5}}},

    {{2, 0}, {6, {1,2,3}}},
    {{2, 1}, {5, {2,3}}},
    {{2, 2}, {3, {3}}},
    {{2, 3}, {1, { }}},
    {{2, 4}, {1, { }}},
    {{2, 5}, {3, {4}}},
    {{2, 6}, {4, {4,5}}},

    {{3, 0}, {8, {1,2,3,4}}},
    {{3, 1}, {7, {2,3,4}}},
    {{3, 2}, {5, {3,4}}},
    {{3, 3}, {3, {4}}},
    {{3, 4}, {1, { }}},
    {{3, 5}, {1, { }}},
    {{3, 6}, {2, {5}}}
};

// {room_idx, room_idx} -> { move_count, hall_blocking }
map<pair<size_t, size_t>, pair<int, vector<size_t>>> room_to_room{
    {{0, 1}, {2, {2}}},
    {{0, 2}, {4, {2,3}}},
    {{0, 3}, {6, {2,3,4}}},

    {{1, 0}, {2, {2}}},
    {{1, 2}, {2, {3}}},
    {{1, 3}, {4, {3,4}}},

    {{2, 0}, {4, {2,3}}},
    {{2, 1}, {2, {3}}},
    {{2, 3}, {2, {4}}},

    {{3, 0}, {6, {2,3,4}}},
    {{3, 1}, {4, {3,4}}},
    {{3, 2}, {2, {4}}},
};

bool is_blocked_rh(size_t room_idx, size_t hall_idx)
{
    auto intermediate = room_to_hall[{room_idx, hall_idx}].second;
    for(auto h : intermediate)
        if(game_board.hall[h] != unoccupied)
            return true;
    return false;
}

bool is_blocked_rr(size_t room_s_idx, size_t room_t_idx)
{
    auto intermediate = room_to_room[{room_s_idx, room_t_idx}].second;
    for(auto h : intermediate)
        if(game_board.hall[h] != unoccupied)
            return true;
    return false;
}

bool is_win()
{
    for(size_t r = 0; r < game_board.rooms.size(); r++)
        if(game_board.rooms[r][room_top] != ('A' + r))
            return false;
    return true;
}

static void next_move();

void check_hallway_to_room()
{
    // can any piece in hallway go home?
    for(size_t h = 0; h < hall_length; h++)
    {
        // identify if piece is present
        char piece = game_board.hall[h];
        if(piece == unoccupied)
            continue;

        // find target room            
        size_t t = piece - 'A';

        // see if room is avail
        if(!room_avail[t])
            continue;

        // ensure it is not blocked
        if(is_blocked_rh(t, h))
            continue;

        //move h -> tr
        game_board.rooms[t][room_pos[t]] = piece;
        game_board.hall[h] = unoccupied;

        // adjust target
        room_pos[t]--;

#ifdef LOG_MOVES
        move_log.push_back(string(1, piece) + " h" + to_string(h) + " -> r" + to_string(t) + ", e +0");
        move_board.push_back(game_board);
#endif
        next_move();

        //undo move and try next
        // adjust target
        room_pos[t]++;        

        // move pieces back
        game_board.rooms[t][room_pos[t]] = unoccupied;
        game_board.hall[h] = piece;

#ifdef LOG_MOVES
        move_log.pop_back();
        move_board.pop_back();
#endif
    }
}

void check_room_to_room()
{
    // can any piece in wrong room go home?
    for(size_t s = 0; s < room_qty; s++)
    {
        if(room_avail[s])
            continue;

        // identify piece;
        char piece = game_board.rooms[s][room_pos[s]];

        // find target room            
        size_t t = piece - 'A';

        // see if room is avail
        if(!room_avail[t])
            continue;

        // ensure it is not blocked
        if(is_blocked_rr(s, t))
            continue;

        //move s -> tr
        game_board.rooms[t][room_pos[t]] = piece;
        game_board.rooms[s][room_pos[s]] = unoccupied;

        // adjust target
        room_pos[t]--;
        // adjust source
        room_avail[s] = true; //consider avail, until proven otherwise
        for(size_t p = room_pos[s] + 1; p < room_depth; p++)
        {
            if(game_board.rooms[s][p] != ('A' + s))
            {
                room_avail[s] = false;
                break;
            }
        }
        if(!room_avail[s])
            room_pos[s]++;

#ifdef LOG_MOVES
        move_log.push_back(string(1, piece) + " r" + to_string(s) + " -> r" + to_string(t) + ", e +0");
        move_board.push_back(game_board);
#endif

        next_move();

        //undo move and try next
        // adjust target
        room_pos[t]++;
        // adjust source
        if(!room_avail[s])
            room_pos[s]--;
        room_avail[s] = false;

        // move pieces back
        game_board.rooms[t][room_pos[t]] = unoccupied;
        game_board.rooms[s][room_pos[s]] = piece;


#ifdef LOG_MOVES
        move_log.pop_back();
        move_board.pop_back();
#endif
    }
}

void check_room_to_hallway()
{
    // can any piece move out of room?
    for(size_t s = 0; s < room_qty; s++)
    {
        if(room_avail[s])
            continue;

        // identify piece;
        char piece = game_board.rooms[s][room_pos[s]];

        for(size_t h = 0; h < hall_length; h++)
        {
            if(game_board.hall[h] != unoccupied)
                continue;

            // ensure it is not blocked
            if(is_blocked_rh(s, h))
                continue;

            //move s -> h
            game_board.hall[h] = piece;
            game_board.rooms[s][room_pos[s]] = unoccupied;


            size_t t = piece - 'A';
            uint64_t steps_direct = room_to_room[{s, t}].first;
            uint64_t steps_through_hall = (room_to_hall[{s, h}].first + room_to_hall[{t, h}].first);
            uint64_t this_energy = (steps_through_hall - steps_direct) * piece_energy[piece];
            delta_energy += this_energy;

            // adjust source
            room_avail[s] = true; //consider avail, until proven otherwise
            for(size_t p = room_pos[s] + 1; p < room_depth; p++)
            {
                if(game_board.rooms[s][p] != ('A' + s))
                {
                    room_avail[s] = false;
                    break;
                }
            }
            if(!room_avail[s])
                room_pos[s]++;

#ifdef LOG_MOVES
            move_log.push_back(string(1, piece) + " r" + to_string(s) + " -> h" + to_string(h) + ", e +" + to_string(this_energy));
            move_board.push_back(game_board);
#endif
            next_move();

            //undo move and try next
            // adjust source
            if(!room_avail[s])
                room_pos[s]--;
            room_avail[s] = false;
            // adjust energy
            delta_energy -= this_energy;
            // move pieces back
            game_board.hall[h] = unoccupied;
            game_board.rooms[s][room_pos[s]] = piece;

#ifdef LOG_MOVES
            move_log.pop_back();
            move_board.pop_back();
#endif
        }
    }
}

static void next_move()
{
    // cout << game_board;
    // cout << '\n';
    
    if(delta_energy >= least_delta_energy_win)
        return;

    //does it win?
    if(is_win())
    {
#ifdef LOG_MOVES
        cout << "win!!!, delta = " << delta_energy << "\n";
        for(size_t i = 0; i < move_log.size(); i++)
        {
            cout << move_log[i] << '\n';
            cout << move_board[i];  //already \n included
        }
        cout << '\n';
        cout << '\n';
#endif

        win_count++;
        least_delta_energy_win = delta_energy;
        return;
    }

    // the remaining functions iterate through options
    // and will call next_move() recursively within

    // can any piece in hallway go home?
    check_hallway_to_room();
    // can any piece in wrong room go home?
    check_room_to_room();
    // can any piece move out of room?
    check_room_to_hallway();
}

uint64_t calc_base_energy()
{
    vector<uint32_t> room_entry_base_count = {1,3,6,10};
    uint64_t energy = 0;

    for(int r = 0; r < room_qty; ++r)
    {
        bool is_home = true;
        for(int i = room_bottom; i >= static_cast<int>(room_top); --i)
        {
            int piece = game_board.rooms[r][i];

            // find target room            
            size_t t = piece - 'A';

            if(is_home && t == r)
                continue; // already home

            if(is_home)
            {
                is_home = false;
                // calculate total for incoming piece counts;
                energy += (room_entry_base_count[i] * piece_energy[r + 'A']);
            }

            energy += (1 + i) * piece_energy[piece];  //just enough to get out of room
            if(t != r)
                energy += room_to_room[{r,t}].first * piece_energy[piece];
        }
    }

    return energy;
}

uint64_t play(vector<string> init)
{
    game_board = Board(init);
    base_energy = calc_base_energy();
    delta_energy = 0;
    least_delta_energy_win = ~0UL;
    win_count = 0;

    for(size_t i = 0; i < room_avail.size(); i++)
        room_avail[i] = false;
    
    next_move();
    
    return win_count ? (base_energy + least_delta_energy_win) : ~0ULL;
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

        uint64_t least_energy = play(room_init);
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
