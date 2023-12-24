#ifndef TIMER_H
#define TIMER_H

#include <string>
#include <chrono>
#include <iostream>
#include <iomanip>

class Timer{
public:
    Timer(std::string name, std::ostream &os = std::cout)
    : name_{name}, os_{os}
    {
        start_ = std::chrono::high_resolution_clock::now();
    }
    ~Timer()
    {
        auto stop = std::chrono::high_resolution_clock::now();
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(stop - start_);
        os_ << "timer \"" << name_ << "\"\t sec: " << (duration.count() / 1000000) << 
               "." <<  std::setfill('0') << std::setw(6) << (duration.count() % 1000000) << std::endl;
    }
private:
    std::chrono::high_resolution_clock::time_point start_;
    std::string name_;
    std::ostream &os_;
};

#endif
