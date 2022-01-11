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
        int position = 0;

        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
        {
            // std::istringstream iss(std::move(file_line));
            // std::string s;
            // while(std::getline(iss, s, ','))
            //     crab_horz.push_back(std::stoi(s));


            int count = 0;
            for(auto c : file_line)
            {                
                if(c == '(')
                    position++;
                if(c == ')')
                    position--;
                
                ++count;
                if(position < 0)
                {
                    std::cout << "basement : " << count << std::endl;
                    return 1;
                }
            }
        }

        std::cout << "position: " << position << std::endl;

        infile.close();
   }
}


