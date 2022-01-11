#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


class Present {
public:
    Present(int length, int width, int hight)
    : l{length}, w{width}, h{hight}
    {
        std::vector<int> v{l,w,h};
        std::sort(v.begin(), v.end());

        a = (2 * ((l * w) + (l * h) + (w * h))) + (v[0] * v[1]);
        r = (2 * (v[0] + v[1])) + (l * w * h);
    }
    int length() const { return l; }
    int width() const { return w; }
    int hight() const { return h; }
    int area() const { return a; }
    int ribbon() const { return r; }
private:
    int l;
    int w;
    int h;
    int a;
    int r;
};

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::vector<Present> presents;


        //parse lines
        std::string file_line;
        while(std::getline(infile, file_line))
        {
            std::istringstream iss(std::move(file_line));
            std::string s;
            std::getline(iss, s, 'x');
            int length = std::stoi(s);
            std::getline(iss, s, 'x');
            int width = std::stoi(s);
            std::getline(iss, s, 'x');
            int height = std::stoi(s);

            presents.push_back(Present(length, width, height));
        }

        int count = 0;
        for(auto p : presents)
            std::cout << "present " << ++count << ": " <<
                p.length() << "x" << p.width() << "x" << p.hight() <<
                "  a=" << p.area() << " r=" << p.ribbon() << std::endl;

        long int sum = 0;
        long int sum_ribbon = 0;
        for(auto p : presents)
        {
            sum += p.area();
            sum_ribbon += p.ribbon();
        }
        std::cout << "total: " << presents.size() << std::endl;
        std::cout << "paper: " << sum << std::endl;
        std::cout << "ribbon: " << sum_ribbon << std::endl;

        infile.close();
   }
}


