#include <bits/stdc++.h>
using namespace std;

uint64_t find_code(uint32_t row, uint32_t col)
{
    uint64_t coded_val = 20151125;
    uint32_t depth = 2;

    while(1)
    {
        for(uint32_t x = 1; x <= depth; x++)
        {
            coded_val = (coded_val * 252533) % 33554393;

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
    if(code != 32451966)
        throw logic_error("example failed");
    code = find_code(1, 5);
    if(code != 10071777)
        throw logic_error("example failed");

    code = find_code(3010, 3019);
    cout << "a) code {3010,3019}: " << code << "\n";

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
