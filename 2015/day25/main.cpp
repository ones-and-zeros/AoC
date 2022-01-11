#include <bits/stdc++.h>
using namespace std;

uint32_t find_code(uint32_t row, uint32_t col)
{
    uint32_t coded_val = 20151125;

    // ex.
    coded_val = 1;

    uint32_t depth = 2;

    while(1)
    {
        for(uint32_t x = 1; x <= depth; x++)
        {
            coded_val++;

            if( col == x &&
                row == (1 + depth - x))
            {
                return coded_val;
            }
        }

        depth++;
    }

    throw domain_error("unexpected loc");
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    // // execute code
    // ifstream infile("input.txt");
    // if(infile.is_open())
    // {
    //     //parse lines
    //     string line;
    //     while(getline(infile, line))
    //     {
    //     }
    //     cout << '\n';
    //     infile.close();
    // }

    // input:
    // To continue, please consult the code grid in the manual.
    // Enter the code at row 3010, column 3019.


    // ex...
    auto code = find_code(4, 2);
    if(code != 12)
        throw logic_error("example failed");
    code = find_code(1, 5);
    if(code != 15)
        throw logic_error("example failed");


    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
