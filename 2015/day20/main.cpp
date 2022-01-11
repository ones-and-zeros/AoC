#include <bits/stdc++.h>
using namespace std;

uint64_t divSum(uint64_t n)
{
    if(n == 1)
      return 1;

    // Sum of divisors
    uint64_t result = 0;
 
    // find all divisors which divides 'num'
    for (uint64_t i = 2; i <= sqrt(n); i++)
    {
        // if 'i' is divisor of 'n'
        if (n % i == 0)
        {
            // if both divisors are same
            // then add it once else add
            // both
            if (i == (n / i))
                result += i;
            else
                result += (i + n/i);
        }
    }
 
    // Add 1 and n to result as above loop
    // considers proper divisors greater 
    // than 1.
    return (result + n + 1);
}


uint64_t divSum_limit50(uint64_t n)
{
    if(n == 1)
      return 1;

    // Sum of divisors
    uint64_t result = 0;
 
    // find all divisors which divides 'num'
    for (uint64_t i = 2; i <= sqrt(n); i++)
    {
        // if 'i' is divisor of 'n'
        if (n % i == 0)
        {
            // if both divisors are same
            // then add it once else add
            // both
            uint64_t other = (n / i);
            if(other <= 50)
                result += i;
            if(i <= 50 && other != i)
                result += other;
        }
    }
 
    // Add 1 and n to result as above loop
    // considers proper divisors greater 
    // than 1.
    return (result + n + 1);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        // //parse lines
        // string line;
        // while(getline(infile, line))
        // {

        // }

#ifdef WORKS_CRUDELY
        for(uint64_t house = 60; house < 1000000; house += 60)
        {
            uint64_t sum = 0;
            for(uint64_t div = 1; div <= house; div++)
            {
                if(house % div == 0)
                {
                    sum += div;
                }
            }
            sum *= 10;

            cout << "house " << house << ": " << sum << "\n";


            if(sum >= 34000000)
            {
                cout << "a) first house w 34000000 gifts: house " << house << "\n";
                break;
            }
        }
#endif // WORKS_CRUDELY



        for(uint64_t house = 1; house < 1000000; house += 1)
        {
            if(divSum(house) * 10 >= 34000000)
            {
                cout << "a) first house w 34000000 gifts: house " << house << "\n";
                break;
            }
        }

        for(uint64_t house = 1; house < 10000000; house += 1)
        {
            if(divSum_limit50(house) * 11 >= 34000000)
            {
                cout << "b) first house w 34000000 gifts elves x 50: house " << house << "\n";
                break;
            }
        }

        cout << '\n';
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
