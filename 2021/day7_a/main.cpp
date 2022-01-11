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
        std::vector<int> crab_horz;

        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
        {
            std::istringstream iss(std::move(file_line));
            std::string s;
            while(std::getline(iss, s, ','))
                crab_horz.push_back(std::stoi(s));
        }

        for(auto c : crab_horz)
            std::cout << c << " ";
        std::cout << std::endl;


        unsigned int least_fuel = ~0;
        size_t least_pos = 0;
        int pos_max = *std::max_element(crab_horz.begin(), crab_horz.end());
        for(int target = 0; target <= pos_max; target++)
        {
            int fuel = 0;
            for(auto c : crab_horz)
            {
                fuel += std::abs(target - c);
                if(fuel >= least_fuel)
                    break;
            }

            if(fuel < least_fuel)
            {
                least_fuel = fuel;
                least_pos = target;
            }

            std::cout << "t" << target << ": " << fuel
                      << "(" << least_pos << ")" << std::endl;
        }


        std::cout << std::endl << "best: " << least_pos <<
                     " (" << least_fuel << ")" << std::endl << std::endl;

        // std::cout << "total fish: " << fish_timers.size() << std::endl;

        infile.close();
   }
}


