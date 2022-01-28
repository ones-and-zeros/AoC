#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

int main()
{
    Timer t1("main");

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t2("file");

        //parse lines
        string line;
        while(getline(infile, line))
        {

        }

        infile.close();
    }
}
