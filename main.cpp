#include <bits/stdc++.h>
using namespace std;


int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        //parse lines
        while(getline(infile, file_line))
        {
            // istringstream iss{file_line};
            // string s1, s2;
            // getline(iss, s1, '-');
            // getline(iss, s2);
            // paths.insert({s1,s2});
            // caves.insert(s1);
            // caves.insert(s2);
        }




        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
