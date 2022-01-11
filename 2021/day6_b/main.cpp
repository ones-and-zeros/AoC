#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>

//1 741 362 314 973

long calc(int val, int days)
{
    long result = 1;

    if(days)
    {
        while(days > val)
        {
            days -= (val + 1);
            val = 6;
            result += calc(8, days);
        }
    }
    
    return result;
}



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


        // for(int day = 0; day < 200; day++)
        // {
        //     long long sum = 0;
        //     for(auto f : fish_timers)
        //         sum += calc(f, day+1);
        //     std::cout << "day " << (day+1) << " total: " << sum << std::endl;
        // }


        std::vector<long> qty_per_fish;

        for(int i = 1; i < 6; i++)
        {
            auto val = calc(i, 256);
            qty_per_fish.push_back(val);
            std::cout << " start " << i << ": " << val << std::endl;
        }

        long long sum = 0;
        for(auto f : fish_timers)
        {
            sum += qty_per_fish[f-1];
        }
        std::cout << " total: " << sum << std::endl;



        // for(auto f : fish_timers)
        //     std::cout << f << " ";
        // std::cout << std::endl;
        
        // for(int day = 0; day < 256; day++)
        // {
        //     size_t new_fish = 0;

        //     std::cout << "start dec" << std::endl;
        //     std::for_each(fish_timers.begin(), fish_timers.end(), [& new_fish](auto& f){f--; if(f<0){f=6;new_fish++;}});

        //     std::cout << "start append" << std::endl;
        //     fish_timers.insert(fish_timers.end()-1, new_fish, 8);

        //     std::cout << "Day " << (day + 1) << ": " << fish_timers.size() << std::endl;

        //     // if(day+1 == 18)
        //     //    std::cout << "day 18: " << fish_timers.size() << std::endl; 
        //     // if(day+1 == 80)
        //     //    std::cout << "day 80: " << fish_timers.size() << std::endl; 
            
        //     // std::cout << "day " << (day + 1) << ": ";
        //     // for(size_t i = 0; i < fish_timers.size(); i++)
        //     //     std::cout << fish_timers[i] << ",";
        //     // std::cout << std::endl; 
        // }

        // std::cout << "total fish: " << fish_timers.size() << std::endl;

        infile.close();
   }
}


