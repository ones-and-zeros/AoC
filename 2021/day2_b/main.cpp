#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        int depth = 0;
        int horz = 0;
        int aim = 0;
        int error = 0;

        std::string line;
        while(std::getline(infile, line))
        {
//            std::cout << " line:" << line << std::endl;

            std::istringstream iss(line);

            std::string s;
            int n;

            if(iss >> s && iss >> n)
            {
                if(s == "forward")
                {
                    horz += n;
                    depth += (aim * n);
                }
                else if(s == "up")
                    aim -= n;
                else if(s == "down")
                    aim += n;

//                std::cout << " h:" << horz << " d:" << depth << std::endl;
            }
            else
            {
                error++;
//                std::cout << "error" << std::endl;
            }
        }
        std::cout << "1: h:" << horz << " d:" << depth << " t:" << horz * depth << std::endl;






        infile.close();
   }
}


