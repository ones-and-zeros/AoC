#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

int main()
{
    Timer t_main("main");

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;
        while(getline(infile, line))
        {
            // todo - parse input
        }
        infile.close();
    }
    else
        throw logic_error("unable to open input file");

    // todo - execute tests
}
