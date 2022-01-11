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
        std::vector<std::string> vec_s;
        std::string line;
        while(std::getline(infile, line))
        {
            vec_s.push_back(line);
        }

        std::vector<std::string> vec_oxygen(vec_s);
        // std::cout << vec_oxygen.size() << ": { ";
        // for(auto v : vec_oxygen)
        //     std::cout << v << " ";
        // std::cout << "}" << std::endl;
        for(size_t digit = 0; digit < vec_oxygen[0].size(); digit++)
        {
            size_t count = 0;
            for(auto s : vec_oxygen)
                if(s[digit] == '1')
                    count++;
            
            char filter = ((count*2) >= vec_oxygen.size() ? '1' : '0');

            auto new_end = std::remove_if( vec_oxygen.begin(),
                            vec_oxygen.end(),
                            [filter, digit] (std::string s) {return s[digit] != filter;} );

            // std::cout << vec_oxygen.size() << " -> ";

            vec_oxygen.resize(new_end - vec_oxygen.begin());

            // std::cout << vec_oxygen.size() << std::endl;
            // std::cout << vec_oxygen.size() << ": { ";
            // for(auto v : vec_oxygen)
            //     std::cout << v << " ";
            // std::cout << "}" << std::endl;

            if(vec_oxygen.size() == 1)
                break;
        }

        // std::cout << vec_oxygen.size() << ": { ";
        // for(auto v : vec_oxygen)
        //     std::cout << v << " ";
        // std::cout << "}" << std::endl;

        std::cout << vec_oxygen.size() << ": " << vec_oxygen[0] << std::endl;



        std::vector<std::string> vec_co2(vec_s);
        // std::cout << vec_co2.size() << ": { ";
        // for(auto v : vec_co2)
        //     std::cout << v << " ";
        // std::cout << "}" << std::endl;
        for(size_t digit = 0; digit < vec_co2[0].size(); digit++)
        {
            size_t count = 0;
            for(auto s : vec_co2)
                if(s[digit] == '1')
                    count++;
            
            char filter = ((count*2) < vec_co2.size() ? '1' : '0');

            auto new_end = std::remove_if( vec_co2.begin(),
                            vec_co2.end(),
                            [filter, digit] (std::string s) {return s[digit] != filter;} );

            // std::cout << vec_co2.size() << " -> ";

            vec_co2.resize(new_end - vec_co2.begin());

            // std::cout << vec_co2.size() << std::endl;
            // std::cout << vec_co2.size() << ": { ";
            // for(auto v : vec_co2)
            //     std::cout << v << " ";
            // std::cout << "}" << std::endl;

            if(vec_co2.size() == 1)
                break;
        }

        // std::cout << vec_co2.size() << ": { ";
        // for(auto v : vec_co2)
        //     std::cout << v << " ";
        // std::cout << "}" << std::endl;

        std::cout << vec_co2.size() << ": " << vec_co2[0] << std::endl;



        int oxy = std::stoi(vec_oxygen[0], nullptr, 2);
        int co2 = std::stoi(vec_co2[0], nullptr, 2);
        int life_support = oxy * co2;
        
        std::cout << "life: " << life_support << " oxy:" << oxy << " co2:" << co2 << std::endl;



        infile.close();
   }
}


