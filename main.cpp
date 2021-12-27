#include <bits/stdc++.h>
using namespace std;

constexpr size_t board_length = 10;
constexpr size_t winning_score = 1000;

size_t a_pos = 0;
size_t b_pos = 0;

size_t a_score = 0;
size_t b_score = 0;

size_t roll_die_qty = 0;

int roll_die()
{
    static int die = 0;

    roll_die_qty++;

    if(die == 0 || die == 100)
        die = 1;
    else
        die++;

    return die;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss(line);
            string junk, pos;
            getline(iss, junk, ':');
            getline(iss, pos);

            if("Player 1" == line.substr(0, 8))
                a_pos = stoi(pos);
            else if("Player 2" == line.substr(0, 8))
                b_pos = stoi(pos);
            else
                throw range_error("input error");
        }

        cout << "Player 1 starts: " << a_pos << '\n';
        cout << "Player 2 starts: " << b_pos << '\n';
        cout << '\n';


        while(a_score < 1000 && b_score < 1000)
        {
            size_t roll_val = 0;
            size_t roll_total = 0;
            
            cout << "1 rolls ";
            for(size_t i = 0; i < 3; i++)
            {
                roll_val = roll_die();
                cout << roll_val << " ";
                roll_total += roll_val;
            }
            cout << ", total " << roll_total;

            a_pos += roll_total;
            while(a_pos > board_length)
                a_pos -= board_length;
            a_score += a_pos;
            cout << ", move to " << a_pos;
            cout << ", score: " << a_score << '\n';
            if(a_score >= winning_score)
                break;


            roll_total = 0;
            cout << "2 rolls ";
            for(size_t i = 0; i < 3; i++)
            {
                roll_val = roll_die();
                cout << roll_val << " ";
                roll_total += roll_val;
            }
            cout << ", total " << roll_total;

            b_pos += roll_total;
            while(b_pos > board_length)
                b_pos -= board_length;
            b_score += b_pos;
            cout << ", move to " << b_pos;
            cout << ", score: " << b_score << '\n';
            if(b_score >= winning_score)
                break;

        }



        size_t loser_score;
        if(a_score >= winning_score)
        {
            cout << "Player 1 wins, die rolled " << roll_die_qty << '\n';
            cout << '\n';
            loser_score = b_score;
        }
        else
        {
            cout << "Player 2 wins, die rolled " << roll_die_qty << ", loser = " << a_score << '\n';
            cout << '\n';
            loser_score = a_score;
        }

        cout <<  " loser = " << loser_score << '\n'; 
        cout << " a - loser x roll_qty: " << (loser_score * roll_die_qty) << '\n';

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
