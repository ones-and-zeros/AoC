#include <bits/stdc++.h>
using namespace std;

#include "timer.h"

using Triangle = array<int,3>;
using Triangles = vector<Triangle>;

bool is_triangle(Triangle triangle_attempt)
{
    partial_sort(triangle_attempt.begin(), triangle_attempt.begin() + 1, triangle_attempt.end(), greater<int>());
    return (triangle_attempt[2] + triangle_attempt[1] > triangle_attempt[0]);
}

uint32_t triangle_count(Triangles potential_triangles)
{
    Timer t("triangle count");
    uint32_t tally = 0;
    
    for(auto t : potential_triangles)
    {
        if(is_triangle(t))
        {
            ++tally;
        }
    }
    return tally;
}

int main()
{
    Timer t_main("main");

    Triangles potential_triangles;

    ifstream infile("input.txt");
    if(infile.is_open())
    {
        Timer t_p("parse input");

        string line;
        while(getline(infile, line))
        {
            istringstream iss{line};
            int a, b, c;
            iss >> a;
            iss >> b;
            iss >> c;
            potential_triangles.push_back({a, b, c});
        }
        infile.close();
    }
    else
        throw logic_error("unable to open input file");
    cout << "\n";

    auto triangle_qty = triangle_count(potential_triangles);
    cout << "part 1 - triangle count: " << triangle_qty << "\n";
    cout << "\n";
}
