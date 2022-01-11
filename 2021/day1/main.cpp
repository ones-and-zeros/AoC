#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, const char* argv[])
{
    std::ifstream infile;
    infile.open("input.txt");
    if(infile.is_open())
    {
        std::string line;
        std::vector<int> v;
        while(std::getline(infile, line))
        {
            std::istringstream iss(line);
            int n;

            while (iss >> n)
            {
                v.push_back(n);
            }
        }

        int count = 0;
        for(size_t i = 0; i < (v.size()-1); i++)
        {
            if(v[i + 1] > v[i])
                count++;
        }
        std::cout << "** " << count << " **" << std::endl;

        std::vector<int> w;
        for(size_t i = 0; i < (v.size()-2); i++)
        {
            w.push_back(v[i] + v[i+1] + v[i+2]);
        }
        int count2 = 0;
        for(size_t i = 0; i < (w.size()-1); i++)
        {
            if(w[i + 1] > w[i])
                count2++;
        }
        std::cout << "** " << count2 << " **" << std::endl;

        infile.close();
   }
}


