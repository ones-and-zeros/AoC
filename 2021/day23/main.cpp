#include <bits/stdc++.h>
using namespace std;

// #############
// #01.2.3.4.56#
// ###A#B#C#D###
//   #1#1#1#1#
//   #2#2#2#2#
//   #3#3#3#3#
//   #########

// Use Dijkstra's algorithm to identify lowest score for each **board-layout**
//
// init
// 1. add input board to queue with score of 0;
//
// runtime
// 1. pop queue (lowest score in priority queue)
// 2. If win
//      end !!!
// 3. If seen-before AND this-energy >= seen-energy
//      goto step 1
// 4. identify each next-move
// 5. add each "next-move, new-score" entry to queue
// 6. repeat at step 1 until empty

// potential ofi(s)
// - calculate base score, as if blocking did not matter,
//   then use delta scores for algorithm
//      - maybe even when going to hall, calc the whole move to final room then.
// - combine solutions to run together
// - simplify the step count logic ??
// - simplify the blocking logic, traverse the string ??

#define SOLUTION_B

#ifdef SOLUTION_B
constexpr size_t room_depth{4};
#else
constexpr size_t room_depth{2};
#endif

constexpr size_t room_qty{4};
constexpr size_t hall_length{7}; //only available spots
constexpr size_t board_length{hall_length + (room_qty * room_depth)};
constexpr size_t offset_room_a{hall_length};
constexpr size_t offset_room_b{hall_length + room_depth};
constexpr size_t offset_room_c{hall_length + (room_depth * 2)};
constexpr size_t offset_room_d{hall_length + (room_depth * 3)};
constexpr char unoccupied{'.'};

map<char, uint32_t> piece_energy { {'A', 1}, {'B', 10}, {'C', 100}, {'D', 1000} };

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

struct Board{
    Board(vector<string> init)
    {
        if(init.size() != room_qty)
            throw range_error("invalid room qty");
        for(auto r : init)
            if(r.size() != room_depth)
                throw range_error("invalid room size");

        char *write = &spaces[offset_room_a];
        for(size_t room = 0; room < room_qty; ++room)
        {
            for(size_t space = 0; space < room_depth; ++space)
            {
                *write++ = init[room][space];
            }
        }
    }
    string spaces{string(board_length, '.')};

    bool is_win()
    {
        for(size_t room = 0; room < room_qty; ++room)
            for(size_t space = 0; space < room_depth; ++space)
                if( ('A' + room) != spaces[offset_room_a + (room * room_depth) + space])
                    return false;
        return true;
    }

    bool is_room_avail(size_t room_idx) const
    {
        const char *rbegin = &spaces[offset_room_b + (room_idx * room_depth) - 1];
        const char *rend = rbegin - room_depth;

        for(auto space = rbegin; space != rend; --space)
        {
            if(*space == unoccupied)
                return true;
            if(*space != 'A' + room_idx)
                return false;
        }
        return true; // (nothing to remove)
    }

    char room_peek(size_t room_idx) const
    {
        const char *begin = &spaces[offset_room_a + (room_idx * room_depth)];
        const char *end = begin + room_depth;

        for(auto space = begin; space != end; ++space)
        {
            if(*space != unoccupied)
                return *space;
        }
        return unoccupied;
    }

    size_t room_pop(size_t room_idx)
    {
        char *begin = &spaces[offset_room_a + (room_idx * room_depth)];
        char *end = begin + room_depth;

        for(auto space = begin; space != end; ++space)
        {
            if(*space != unoccupied)
            {
                *space = unoccupied;
                return space + 1 - begin; // steps to exit
            }
        }
        return 0;
    }

    size_t room_push(size_t room_idx, char piece)
    {
        char *rbegin = &spaces[offset_room_b + (room_idx * room_depth) - 1];
        char *rend = rbegin - room_depth;

        for(auto space = rbegin; space != rend; --space)
        {
            if(*space == unoccupied)
            {
                *space = piece;
                return space - rend; // steps to enter
            }  
        }
        return 0;
    }

    bool is_blocked_rh(size_t room_idx, size_t hall_idx) const
    {
        auto blockers = room_to_hall[{room_idx, hall_idx}].second;
        for(auto h : blockers)
            if(spaces[h] != unoccupied)
                return true;
        return false;
    }

    bool is_blocked_rr(size_t room_s_idx, size_t room_t_idx)
    {
        auto blockers = room_to_room[{room_s_idx, room_t_idx}].second;
        for(auto h : blockers)
            if(spaces[h] != unoccupied)
                return true;
        return false;
    }

    int32_t move_hall_to_room(size_t h)
    {
        // identify if piece is present
        char piece = spaces[h];
        if(piece == unoccupied)
            return 0;

        // find target room            
        size_t t = piece - 'A';

        // see if room is avail
        if(!is_room_avail(t))
            return 0;

        // ensure it is not blocked
        if(is_blocked_rh(t, h))
            return 0;

        // move
        spaces[h] = unoccupied;
        auto step_count = room_to_hall[{t, h}].first;
        step_count += room_push(t, piece);

        return  step_count * piece_energy[piece];
    }

    uint32_t move_room_to_room(size_t room_src)
    {
        if(is_room_avail(room_src))
            return 0;
        auto piece = room_peek(room_src);
        if(piece == unoccupied)
            return 0;

        // find target room            
        size_t t = piece - 'A';

        // see if room is avail
        if(!is_room_avail(t))
            return 0;

        // ensure it is not blocked
        if(is_blocked_rr(room_src, t))
            return 0;

        // move
        auto step_count = room_to_room[{room_src, t}].first;
        step_count += room_pop(room_src);
        step_count += room_push(t, piece);

        return step_count * piece_energy[piece];
    }

    uint32_t move_room_to_hallway(size_t room_idx, size_t hall_idx)
    {
        if(is_room_avail(room_idx))
            return 0;
        auto piece = room_peek(room_idx);
        if(piece == unoccupied)
            return 0;
        if(spaces[hall_idx] != unoccupied)
            return 0;

        // ensure it is not blocked
        if(is_blocked_rh(room_idx, hall_idx))
            return 0;

        // move
        auto step_count = room_to_hall[{room_idx, hall_idx}].first;
        step_count += room_pop(room_idx);
        spaces[hall_idx] = piece;

        return step_count * piece_energy[piece];
    }
};

bool operator<(const Board a, const Board b)
{
    return a.spaces < b.spaces;
}

struct Move_stat{
    Move_stat(uint32_t energy, Board board)
    : board{board}, energy{energy}
    {
    }
    uint32_t energy;
    Board board;
};

bool operator>(const Move_stat a, const Move_stat b)
{
    return a.energy > b.energy;
}

using Move_queue = priority_queue<Move_stat,vector<Move_stat>, greater<Move_stat>>;

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
    os << "#" << b.spaces[0] << b.spaces[1];
    os << "." << b.spaces[2];
    os << "." << b.spaces[3];
    os << "." << b.spaces[4];
    os << "." << b.spaces[5] << b.spaces[6] << "#\n";
    os << "###" << b.spaces[offset_room_a];
    os <<   "#" << b.spaces[offset_room_b];
    os <<   "#" << b.spaces[offset_room_c];
    os <<   "#" << b.spaces[offset_room_d] << "###\n";
    for(size_t i = 1; i < room_depth; ++i)
    {
        os << "  #" << b.spaces[offset_room_a + i] <<
                "#" << b.spaces[offset_room_b + i] <<
                "#" << b.spaces[offset_room_c + i] <<
                "#" << b.spaces[offset_room_d + i] << "#\n";
    }
    os << "  #########\n";

    return os;
}

void queue_up_hallway_to_room(Move_queue &move_queue, Move_stat current_move)
{
    // can any piece in hallway go home?
    for(size_t h = 0; h < hall_length; ++h)
    {
        Board board = current_move.board;
        auto energy_used = board.move_hall_to_room(h);
        if(energy_used)
            move_queue.push({energy_used + current_move.energy, board});
    }
}

void queue_up_room_to_x(Move_queue &move_queue, Move_stat current_move)
{
    for(size_t r = 0; r < room_qty; ++r)
    {
        Board board = current_move.board;

        // can any piece in wrong room go home?
        auto energy_used = board.move_room_to_room(r);
        if(energy_used)
        {
            move_queue.push({energy_used + current_move.energy, board});
            continue;
        }
        // ...if not...
        // can any piece move out of room to hall?
        for(size_t h = 0; h < hall_length; ++h)
        {
            Board board = current_move.board;
            auto energy_used = board.move_room_to_hallway(r, h);
            if(energy_used)
            {
                move_queue.push({energy_used + current_move.energy, board});
            }
        }
    }
}

uint64_t play(vector<string> init)
{
    map<Board, uint64_t> found;
    Move_queue move_queue;
    move_queue.push({0, Board(init)});
    while(move_queue.size() > 0)
    {
        auto move = move_queue.top();
        move_queue.pop();

        // cout << move.energy << "\n";
        // cout << move.board;
        // cout << endl;

        if(move.board.is_win())
            return move.energy;    // <---- solution !!!

        if(found.end() == found.find(move.board))
            found[move.board] = move.energy;
        else if(move.energy < found[move.board]) 
            found[move.board] = move.energy;
        else
            continue;
        
        // can any piece in hallway go home?
        queue_up_hallway_to_room(move_queue, move);
        // can any piece in wrong room come out?
        queue_up_room_to_x(move_queue, move);
    }

    // no solution found
    move_queue = Move_queue{};
    return ~0ULL;
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

#ifdef SOLUTION_B
        cout << " b - least power: " << least_energy << '\n';
#else
        cout << " a - least power: " << least_energy << '\n';
#endif
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
