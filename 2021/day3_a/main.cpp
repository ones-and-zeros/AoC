#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        size_t input_qty = 0;
        int digit_qty = 0;

        std::vector<int> v;
        std::string line;
        if(std::getline(infile, line))
        {
            input_qty++;
            digit_qty = line.size();
            for(size_t i = 0; i < line.size(); i++)
                if(line[i] == '1')
                    v.push_back(1);
                else if(line[i] == '0')
                    v.push_back(0);
        }

        std::cout << "v.size(): " << v.size() << std::endl;

        while(std::getline(infile, line))
        {
//            std::cout << " line:" << line << std::endl;

            input_qty++;

            for(size_t i = 0; i < line.size(); i++)
                if(line[i] == '1')
                    v[i]++;
        }

        std::string gamma_s;
        std::string epsilon_s;
        for(auto d : v)
        {
            if((d * 2) >= input_qty)
            {
                gamma_s += '1';
                epsilon_s += '0';
            }
            else
            {
                gamma_s += '0';
                epsilon_s += '1';
            }
        }

        int gamma_rate = std::stoi(gamma_s, nullptr, 2);
        int epsilon_rate = std::stoi(epsilon_s, nullptr, 2);
        int power = gamma_rate * epsilon_rate;

        std::cout << "power:" << power << " gamma:" << gamma_rate << " epsilon:" << epsilon_rate << std::endl;



        infile.close();
   }
}


