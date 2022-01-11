#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <algorithm>
#include <bitset>
#include <map>


int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::vector<std::string> rows_s;
        std::vector<std::pair<int,int>> low_points;

        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
            rows_s.push_back(file_line);

        // find low points
        for(int x = 0; x < rows_s.size(); x++)
        {
            for(int y = 0; y < rows_s[0].size(); y++)
            {
                int loc_val = rows_s[x][y];

                if((y >= 1) && (rows_s[x][y - 1] <= loc_val))
                    continue;
                if((y < (rows_s[0].size() - 1)) && (rows_s[x][y + 1] <= loc_val))
                    continue;

                if((x >= 1) && (rows_s[x - 1][y] <= loc_val))
                    continue;
                if((x < (rows_s.size() - 1)) && (rows_s[x + 1][y] <= loc_val))
                    continue;

                low_points.push_back({x,y});
            }
        }

        for(auto s : rows_s)
            std::cout << s << std::endl;
        std::cout << std::endl;

        std::cout << "low points: " << std::endl;
        for(auto p : low_points)
            std::cout << " {" << p.first << "," << p.second << "} = " << rows_s[p.first][p.second] << std::endl;
        std::cout << std::endl;

        int risk_total = 0;
        for(auto p : low_points)
            risk_total += (rows_s[p.first][p.second] - '0') + 1;
        std::cout << "Total Risk: " << risk_total << std::endl;


//        std::vector{}



        // std::cout << std::endl;
        // std::cout << "total: " << total << std::endl;

        std::cout << std::endl;
        infile.close();
   }
}


