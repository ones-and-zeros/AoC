#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

struct Point
{
    int x;
    int y;
};

struct Line
{
    Point a;
    Point b;
};

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::string file_line;
        size_t max_x = 0;
        size_t max_y = 0;

        //parse lines
        std::vector<Line> lines;
        while(std::getline(infile, file_line))
        {
            std::istringstream iss(std::move(file_line));
            std::string s;

            Line l;
            std::getline(iss, s, ',');
            l.a.x = std::stoi(s);
            std::getline(iss, s, ' ');
            l.a.y = std::stoi(s);
            std::getline(iss, s, ' ');
            std::getline(iss, s, ',');
            l.b.x = std::stoi(s);
            std::getline(iss, s);
            l.b.y = std::stoi(s);

            //only use horizontal or vertical lines
            if((l.a.x == l.b.x) || (l.a.y == l.b.y))
                lines.push_back(l);
            else if(std::abs(l.a.x - l.b.x) == std::abs(l.a.y - l.b.y))
                lines.push_back(l);
        }

        //find extents
        for(auto l : lines)
        {
            if(l.a.x > max_x)
                max_x = l.a.x;
            if(l.b.x > max_x)
                max_x = l.b.x;
            if(l.a.y > max_y)
                max_y = l.a.y;
            if(l.b.y > max_y)
                max_y = l.b.y;
        
//            std::cout << l.a.x << "-" << l.a.y << " to " << l.b.x << "-" << l.b.y << std::endl;
        }

        // make grid to size of extents
        std::vector<std::vector<int>> grid( max_x + 1, std::vector<int>(max_y + 1) );

        // std::cout << "max_x:" << max_x << " max_y:" << max_y << std::endl; 
        // std::cout << "grid_x:" << grid.size() << " grid_y:" << grid[1].size() << std::endl; 

        //apply lines to grid`
        for(auto l : lines)
        {
            int len = (l.a.x != l.b.x) ? abs(l.a.x-l.b.x)+1 : abs(l.a.y-l.b.y)+1;
            int dir_x = (l.a.x < l.b.x) ?  1 :
                        (l.a.x > l.b.x) ? -1 :
                                           0;
            int dir_y = (l.a.y < l.b.y) ?  1 :
                        (l.a.y > l.b.y) ? -1 :
                                           0;

            int x = l.a.x;
            int y = l.a.y;
            for(int i = 0; i < len; i++)
            {
                grid[x][y]++;
                x += dir_x;
                y += dir_y;
            }
        }

        // //print output
        // std::cout << "applied:" << std::endl;
        // for(size_t y = 0; y <= max_y; y++)
        // {
        //     for(size_t x = 0; x <= max_x; x++)
        //     {
        //         if(grid[x][y])
        //             std::cout << grid[x][y];
        //         else
        //             std::cout << '.';
        //     }
            
        //     std::cout << std::endl;            
        // }
        // std::cout << std::endl;

        size_t count = 0;
        for(auto v : grid)
            for(auto d : v)
                if(d > 1)
                    count++;
        std::cout << "result: " << count << std::endl;

        infile.close();
   }
}


