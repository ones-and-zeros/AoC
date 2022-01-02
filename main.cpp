// build and run with
// g++ main.cpp md5.cpp -o main && ./main

#include <bits/stdc++.h>
using namespace std;

vector<string> invalid_seq {
    {"ab"},
    {"cd"},
    {"pq"},
    {"xy"}
};

bool at_least_3_vowels(string word)
{
    uint32_t vowel_count = 0;
    for(auto w : word)
        if( w == 'a' || w == 'e' || w == 'i' || w == 'o' || w == 'u' )
            if(++vowel_count == 3)
                return true;
    return false;
}

bool has_dub_letter(string word)
{
    for(size_t i = 0; i < word.size() - 1; i++)
        if(word[i] == word[i + 1])
            return true;
    return false;
}

bool all_valid_seq(string word)
{
    for(auto seq : invalid_seq)
        if(word.size() > word.find(seq))
            return false;
    return true;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> words;

        //parse lines
        while(getline(infile, line))
        {
            words.push_back(line);
        }

        uint64_t nice_count = 0;
        for(auto w : words)
        {
            if( at_least_3_vowels(w) &&
                has_dub_letter(w) &&
                all_valid_seq(w) )
            {
                nice_count++;
                cout << w << ": nice\n"; 
            }
            else
                cout << w << ": naughty\n"; 
        }

        cout << "a: total nice = " << nice_count << "\n";
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
