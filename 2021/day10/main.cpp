#include <bits/stdc++.h>
using namespace std;

map<char,char> lookup({
    {'(', ')'},
    {'[', ']'},
    {'{', '}'},
    {'<', '>'}
});

map<char,int> score({
    {')', 3},
    {']', 57},
    {'}', 1197},
    {'>', 25137}
});

map<char,int> inc_score({
    {')', 1},
    {']', 2},
    {'}', 3},
    {'>', 4}
});

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;   
        long error_score = 0;
        vector<long> incomplete_scores;

        //parse lines
        while(getline(infile, file_line))
        {
            stack<char> expected;
            for(auto c : file_line)
            {
                if( lookup.find(c) != lookup.end())
                    expected.push(lookup[c]);
                else if(expected.size() && (c == expected.top())) 
                    expected.pop();
                else
                {
                    // cout << "error line " << file_line << ": " <<
                    //       "expected '" << expected.top() << "' but found '" << c << "'" << endl;
                    error_score += score[c];
                    //flush
                    while(expected.size())
                        expected.pop();
                    break;
                }
            }
            //check for incomplete lines
            if( expected.size() )
            {
                long sum = 0;
                while(expected.size())
                {
                    sum = (sum * 5) + inc_score[expected.top()];
                    expected.pop();
                }
                incomplete_scores.push_back(sum);
//                cout << "incomplete on line " << file_line << " => " << sum << endl;
            }
        }

        cout << "A = error score: " << error_score << endl;

        sort(incomplete_scores.begin(), incomplete_scores.end());
        cout << "B = middle autocomplete score: " << incomplete_scores[incomplete_scores.size()>>1] << 
            " (out of " << incomplete_scores.size() << ")" << endl;

        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
