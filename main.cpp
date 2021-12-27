#include <bits/stdc++.h>
using namespace std;

constexpr size_t board_length = 10;
constexpr size_t winning_score = 1000;

size_t player_1 = 0;
size_t player_2 = 1;
vector<size_t> other = {player_2, player_1};

vector<size_t> pos = {{0}, {0}};
vector<size_t> score = {{0}, {0}};

size_t die_val = 0;
size_t roll_die_qty = 0;

int roll_die()
{
    if(die_val == 0 || die_val == 100)
        die_val = 1;
    else
        die_val++;

    return die_val;
}

void game_a_move(int player)
{
    vector<int> rolls;
    for(size_t i = 0; i < 3; i++)
        rolls.push_back(roll_die());

    int roll_total = 0;
    for(auto r : rolls)
        roll_total += r;

    roll_die_qty+= 3;

    /* move player on board */
    pos[player] += roll_total;
    while(pos[player] > board_length)
        pos[player] -= board_length;
    /* tally score per board position */
    score[player] += pos[player];

    /* display stats */
    cout << "P" << player+1 << " rolls";
    for(auto r : rolls)
        cout << " " << r;
    cout << ", total " << roll_total;
    cout << ", move to " << pos[player];
    cout << ", score: " << score[player] << '\n';
    if(score[player]  >= winning_score)
    {
        cout << "P" << player+1 << " wins!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n";

        cout << " a - loser-score x roll_qty: " << (score[other[player]] * roll_die_qty) << '\n';
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

    die_val = 0;
    roll_die_qty = 0;

    cout << "New game" << '\n';
    cout << "Player 1 starts: " << pos[player_1] << '\n';
    cout << "Player 2 starts: " << pos[player_2] << '\n';
    cout << '\n';

    game_a_move(player_1);
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
