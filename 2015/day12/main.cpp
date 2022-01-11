#include <bits/stdc++.h>
using namespace std;

bool ignore_red_objs_ = false;

int64_t parse_val(istringstream& iss)
{
    int64_t val = 0;
    char c_in;
    iss >> c_in;

    char delim = '\0';
    if(c_in == '{')
        delim = '}';
    else if(c_in == '[')
        delim = ']';
    else
        throw range_error("unexpected delimiter start");

    while(iss.rdbuf()->in_avail() != 0)
    {
        iss >> c_in;

        if(c_in == delim)
            return val; //proper return;

        if(c_in == '}' || c_in == ']')
            throw range_error("unexpected delimiter end");

        if(c_in == '{' || c_in == '[')
        {
            iss.putback(c_in);
            val += parse_val(iss);
            continue;
        }

        if( ignore_red_objs_ &&
            delim == '}' &&
            c_in == '\"' )
        {
            string word;
            getline(iss, word, '\"');
            if(word != "red")
                continue;
            // red value within object
            // flush rest of object, until delimiter
            iss >> c_in;
            while(c_in != delim)
            {
                if(c_in == '{' || c_in == '[')
                {
                    iss.putback(c_in);
                    (void)parse_val(iss); //do not use return val
                }
                iss >> c_in;
            }
            return 0;
        }

        if(!isdigit(c_in) && c_in != '-')
            continue;

        iss.putback(c_in);
        int n;
        iss >> n;
        val += n;
    }

    throw range_error("unexpected delimiter end");
//    return 0;
}

int64_t str_sum(string s, bool ignore_red_objs)
{
    ignore_red_objs_ = ignore_red_objs;
    istringstream iss{s};
    return parse_val(iss);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<string> s_in;

        //parse lines
        while(getline(infile, line))
        {
            s_in.push_back(line);
        }

        for(auto s : s_in)
        {
            cout << "a) sum: " << str_sum(s, false) << "\n";
            cout << "b) sum: " << str_sum(s, true) << "\n";
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
