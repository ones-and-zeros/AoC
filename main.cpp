// build and run with
// g++ main.cpp md5.cpp -o main && ./main

#include <bits/stdc++.h>
using namespace std;

#include "md5.h"

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        string secret_key;
        //parse lines
        while(getline(infile, line))
        {
            secret_key = line;
        }

        
        for(uint64_t n = 100000; true; n++)
        {
            string attempt = secret_key + to_string(n);
            string result = md5(attempt);
            if(result.substr(0, 5) != string(5,'0'))
                continue;
            cout << "a: " << n << " -> " << result << "\n";
            break;
        }

        for(uint64_t n = 100000; true; n++)
        {
            string attempt = secret_key + to_string(n);
            string result = md5(attempt);
            if(result.substr(0, 6) != string(6,'0'))
                continue;
            cout << "b: " << n << " -> " << result << "\n";
            break;
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
