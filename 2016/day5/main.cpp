#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

int main()
{
    Timer t_main("main");

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            // todo
        }

        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

}
