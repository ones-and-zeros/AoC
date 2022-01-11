#include <bits/stdc++.h>
using namespace std;


bool is_valid_pwd(string attempt)
{
    // rule # 1 - must include one increasing straight
    // of at least three letters, like abc, bcd,
    // cde, and so on, up to xyz.
    // They cannot skip letters; abd doesn't count.
    bool straight_occured = false;
    for(size_t i = 0; i + 2 < attempt.size(); i++)
    {
        if((attempt[i] + 1) == attempt[i + 1] &&
           (attempt[i] + 2) == attempt[i + 2] )
        {
            straight_occured = true;
            break;
        }
    }
    if(false == straight_occured)
        return false;

    // rule # 2 - cannot contain 'i', 'o', or 'l'
    if( attempt.find_first_of("iol") < attempt.size())
        return false;

    // rule # 3 - Passwords must contain at least two different,
    // non-overlapping pairs of letters, like aa, bb, or zz.
    size_t char_pair_count = 0;
    for(size_t i = 0; i + 1 < attempt.size(); i++)
    {
        if(attempt[i] == attempt[i + 1])
        {
            char_pair_count++;
            i++; // skip past 2nd char, no overlapping allowed
        }
    }
    if(char_pair_count < 2)
        return false;

    return true;
}

void string_inc(string& s)
{
    for(int i = s.size() - 1; i > 0; i--)
    {
        s[i]++;
        if(s[i] <= 'z')
            break;
        s[i] = 'a';
    }    
}

string next_valid_pwd(string s)
{
    while(1)
    {
        string_inc(s);
        if(is_valid_pwd(s))
            break;
    }

    return s;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> s_in;

        //parse lines
        while(getline(infile, line))
        {
            s_in.push_back(line);
        }

        for(auto s : s_in)
        {
            cout << s << ": " << is_valid_pwd(s);
            
            cout << ",  next: " << next_valid_pwd(s);
            cout << ",  next: " << next_valid_pwd(next_valid_pwd(s)) << "\n";
        }

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
