#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::string file_line;

        std::vector<int> fish_timers;

        //parse lines
        while(std::getline(infile, file_line))
        {
            std::istringstream iss(std::move(file_line));
            std::string s;
            while(std::getline(iss, s, ','))
                fish_timers.push_back(std::stoi(s));
        }

        for(auto f : fish_timers)
            std::cout << f << " ";
        std::cout << std::endl;
        
        for(int day = 0; day < 80; day++)
        {
            int new_fish = 0;
            for(auto& f : fish_timers)
            {
                if(f == 0)
                {
                    f = 6;
                    new_fish++;
                }
                else
                    f--;
            }

            for(int i=0; i<new_fish; i++)
                fish_timers.push_back(8);

            // if(day+1 == 18)
            //    std::cout << "day 18: " << fish_timers.size() << std::endl; 
            // if(day+1 == 80)
            //    std::cout << "day 80: " << fish_timers.size() << std::endl; 
            
            // std::cout << "day " << (day + 1) << ": ";
            // for(size_t i = 0; i < fish_timers.size(); i++)
            //     std::cout << fish_timers[i] << ",";
            // std::cout << std::endl; 
        }

        std::cout << "total fish: " << fish_timers.size() << std::endl;

        infile.close();
   }
}


