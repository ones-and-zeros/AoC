#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <algorithm>


//day4-a
//attempt failed 10052 was too low

constexpr size_t board_size = 5;


struct Position {
    int value = 0;
    bool active = false;
};

struct Board {
    Position square[board_size][board_size] = {0};
};

bool win(Board& board)
{
    //check rows
    for(size_t x = 0; x < board_size; x++)
    {
        int count = 0;
        for(size_t y = 0; y < board_size; y++)
            if(board.square[x][y].active)
                count++;
        if(count == board_size)
            return true;
    }

    //check columns
    for(size_t y = 0; y < board_size; y++)
    {
        int count = 0;
        for(size_t x = 0; x < board_size; x++)
            if(board.square[x][y].active)
                count++;
        if(count == board_size)
            return true;
    }

    // //check diag_1
    // {
    //     int count = 0;
    //     for(size_t x = 0; x < board_size; x++)
    //     {
    //         if(board.square[x][x].active)
    //             count++;
    //     }
    //     if(count == board_size)
    //         return true;
    // }

    // //check diag_2
    // {
    //     int count = 0;
    //     for(size_t x = 0; x < board_size; x++)
    //     {
    //         if(board.square[x][(board_size-1)-x].active)
    //             count++;
    //     }
    //     if(count == board_size)
    //         return true;
    // }

    return false;
}

int main(int argc, const char* argv[])
{
    std::ifstream infile("input.txt");
    if(infile.is_open())
    {
        std::string line;

        //read first line
        if(!std::getline(infile, line))
            return 1; //error            
        //parse "bingo balls" input values
        std::vector<int> bingo_balls;
        std::istringstream ss(std::move(line));
        for (std::string value; std::getline(ss, value, ','); )
            bingo_balls.push_back(std::stoi(value));

        // for(auto s : bingo_balls)
        //     std::cout << s << std::endl;



        std::vector<Board> boards;
        while(std::getline(infile, line))
        {
            if(line.empty())
                continue;

            Board board;
            for(size_t x = 0; x < board_size; x++)
            {
                std::istringstream ss(std::move(line));
                for(size_t y = 0; y < board_size; y++)
                {
                    ss >> board.square[x][y].value;
                }
                //get next line
                if(!std::getline(infile, line))
                    break; // EOF
            }
            boards.push_back(board);

        }


        for(auto ball : bingo_balls)
        {
            std::cout << "Play: " << ball << std::endl;

            // mark boards
            for(auto& b : boards)
            {
                for(size_t x = 0; x < board_size; x++)
                {
                    for(size_t y = 0; y < board_size; y++)
                    {
                        if(b.square[x][y].value == ball)
                            b.square[x][y].active = true;
                    }
                }
            }

            //check for winner
            for(auto& b : boards)
            {
                if(win(b))
                {
                    std::cout << "Winner: " << std::endl;
                    for(size_t x = 0; x < board_size; x++)
                    {
                        for(size_t y = 0; y < board_size; y++)
                        {
                            Position pos = b.square[x][y];
                            std::cout << pos.value << "=" << (pos.active ? "1" : "0") << " ";
                        }
                        std::cout << std::endl;
                    }
                    std::cout << std::endl;

                    int sum = 0;
                    for(size_t x = 0; x < board_size; x++)
                    {
                        for(size_t y = 0; y < board_size; y++)
                        {
                            if(!b.square[x][y].active)
                                sum += b.square[x][y].value;
                        }
                    }
                    std::cout << " sol: " << (sum * ball) << std::endl;

                    return 1;
                }

            }







            // std::cout << "Play: " << ball << std::endl;
            // for(auto& b : boards)
            // {
            //     for(size_t x = 0; x < board_size; x++)
            //     {
            //         for(size_t y = 0; y < board_size; y++)
            //         {
            //             Position pos = b.square[x][y];
            //             std::cout << pos.value << "=" << (pos.active ? "1" : "0") << " ";
            //         }
            //         std::cout << std::endl;
            //     }
            //     std::cout << std::endl;
            // }
        }


        infile.close();
   }
}


