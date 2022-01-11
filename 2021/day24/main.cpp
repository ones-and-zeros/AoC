#include <bits/stdc++.h>
using namespace std;

            //   0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13
vector<int> r {  1,  1,  1, 26,  1,  1, 26,  1, 26,  1, 26, 26, 26, 26 };
vector<int> s { 15, 14, 11,-13, 14, 15, -7, 10,-12, 15,-16, -9, -8, -8 };
vector<int> t {  4, 16, 14,  3, 11, 13, 11,  7, 12, 15, 13,  1, 15,  4 };

vector<long long> max_z;

string answer{"12345678901234"};

void process(size_t depth, long long z)
{
    if(depth == 14)
    {
        if(z == 0)
            cout << answer << '\n';
        return;
    }

    if(z >= max_z[depth])
        return;

    int x = z % 26;
    z /= r[depth];

    long long z_entry = z;
    for(int w=1; w <=9; w++)
    {
        if( false == ((x + s[depth]) == w) )
            z = (z * 26) + w + t[depth];

        answer[depth] = w + '0';
        process(depth+1, z);
        z = z_entry;
    }
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    long long total = 1;
    for(int i = r.size() - 1; i >= 0; i--)
    {
        total = (total * r[i]);
        max_z.insert(max_z.begin(), total);
    }

    // for(auto m : max_z)
    //     cout << m << '\n';

    process(0, 0);

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
