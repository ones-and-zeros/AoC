#include <bits/stdc++.h>
using namespace std;

constexpr size_t board_length = 10;
constexpr size_t winning_a_score = 1000;
constexpr size_t winning_b_score = 21;

size_t player_1 = 0;
size_t player_2 = 1;
vector<size_t> other = {player_2, player_1};

vector<uint64_t> universe_wins = {{0},{0}};
vector<size_t> pos = {{0}, {0}};
vector<size_t> score = {{0}, {0}};

size_t die_a_val = 0;
size_t roll_die_a_qty = 0;

int roll_die()
{
    if(die_a_val == 0 || die_a_val == 100)
        die_a_val = 1;
    else
        die_a_val++;

    return die_a_val;
}

void game_a_move(int player)
{
    vector<int> rolls;
    for(size_t i = 0; i < 3; i++)
        rolls.push_back(roll_die());

    int roll_total = 0;
    for(auto r : rolls)
        roll_total += r;

    roll_die_a_qty+= 3;

    /* move player on board */
    pos[player] += roll_total;
    while(pos[player] > board_length)
        pos[player] -= board_length;
    /* tally score per board position */
    score[player] += pos[player];

    /* display stats */
    // cout << "P" << player+1 << " rolls";
    // for(auto r : rolls)
    //     cout << " " << r;
    // cout << ", total " << roll_total;
    // cout << ", move to " << pos[player];
    // cout << ", score: " << score[player] << '\n';
    if(score[player]  >= winning_a_score)
    {
        cout << "Player " << player+1 << " wins!!!\n";

        cout << " a - loser-score x roll_qty: " << (score[other[player]] * roll_die_a_qty) << '\n';
        return;
    }

    game_a_move(other[player]);
}

void play_game_a(size_t start_1, size_t start_2)
{
    pos[player_1] = start_1;
    pos[player_2] = start_2;

    for(size_t p = player_1; p <= player_2; p++)
        score[p] = 0;

    die_a_val = 0;
    roll_die_a_qty = 0;

    cout << "New game-a" << '\n';
    cout << "Player 1 starts: " << pos[player_1] << '\n';
    cout << "Player 2 starts: " << pos[player_2] << '\n';

    game_a_move(player_1);

    cout << '\n';
}

static uint64_t game_depth = 0;

void game_b_move(int player)
{
    vector<size_t> last_pos = pos;
    vector<size_t> last_score = score;

    map<size_t, pair<uint64_t, uint64_t>> outcomes;

    for(size_t x = 1; x <= 3; x++)
    {
        for(size_t y = 1; y <= 3; y++)
        {
            for(size_t z = 1; z <= 3; z++)
            {
                //revert score\pos and try next roll combo;
                score = last_score;
                pos = last_pos;

                size_t roll = x + y + z;
   
                auto result = outcomes.find(roll);
                if(outcomes.end() != result)
                {
                    universe_wins[player] += result->second.first;
                    universe_wins[other[player]] += result->second.second;
                    continue;
                }

                /* move player on board */
                pos[player] += roll;
                while(pos[player] > board_length)
                    pos[player] -= board_length;
                /* tally score per board position */    
                score[player] += pos[player];

                /* display stats */
                // cout << string(game_depth , ' ');
                // cout << "P" << player+1 << " rolls " << roll;
                // cout << ", move to " << pos[player];
                // cout << ", score: " << score[player] << '\n';

                uint64_t last_u_player = universe_wins[player];
                uint64_t last_u_other = universe_wins[other[player]];

                if(score[player] >= winning_b_score)
                {
                    universe_wins[player]++;

                    // cout << string(game_depth , ' ');
                    // cout << " p" << player + 1 << " win " << universe_wins[player] << ": ";
                    // cout << score[player] << " to " << score[other[player]] << '\n';
                }
                else
                {                    
                    game_depth++;
                    game_b_move(other[player]);
                    game_depth--;
                }

                outcomes.insert({roll, {universe_wins[player] - last_u_player, universe_wins[other[player]] - last_u_other}});
            }
        }
    }
}

void play_game_b(size_t start_1, size_t start_2)
{
    pos[player_1] = start_1;
    pos[player_2] = start_2;

    for(size_t p = player_1; p <= player_2; p++)
    {
        score[p] = 0;
        universe_wins[p] = 0;
    }

    cout << "New game-b" << '\n';
    cout << "Player 1 starts: " << pos[player_1] << '\n';
    cout << "Player 2 starts: " << pos[player_2] << '\n';

    game_b_move(player_1);

    cout << "  Player 1 wins in x universes: " << universe_wins[player_1] << '\n';
    cout << "  Player 2 wins in x universes: " << universe_wins[player_2] << '\n';
    cout << "                   total games: " << universe_wins[player_1] + universe_wins[player_2] << '\n';
    cout << '\n';
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<size_t> start_pos;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss(line);
            string junk, pos;
            getline(iss, junk, ':');
            getline(iss, pos);

            if("Player " == line.substr(0, 7))
                start_pos.push_back(stoi(pos));
            else
                throw range_error("input error");
        }

        play_game_a(start_pos[player_1], start_pos[player_2]);

        play_game_b(start_pos[player_1], start_pos[player_2]);

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
