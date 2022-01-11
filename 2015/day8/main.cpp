#include <bits/stdc++.h>
using namespace std;


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

        // for(auto s : s_in)
        // {
        //     cout << s << "\n";
        // }

        uint64_t char_count = 0;
        uint64_t memory_count = 0;
        vector<string> s_out;
        size_t pos = 0;
        for(auto s : s_in)
        {
            if(s[0] != '"' || s[s.size() - 1] != '"')
                throw range_error("missing quotes");

            string out = s.substr(1, s.size() - 2);

            for(size_t i = 0; i + 1 < out.size(); i++)
            {
                if(out[i] != '\\')
                    continue;

                if(out[i + 1] == '\\')
                    out.replace(i, 2, "\\");
                else if(out[i + 1] == '\"')
                    out.replace(i, 2, "\"");
                else if(i + 3 < out.size() && out[i + 1] == 'x')
                    out.replace(i, 4, string(1, stoul(out.substr(i+2, 2), nullptr, 16)));
            }
            s_out.push_back(out);

            char_count += s.size();
            memory_count += out.size();
        }
        cout << "a) literal - formatted = " << char_count - memory_count << "\n";

        size_t full_count = 0;
        s_out.clear();
        pos = 0;
        for(auto s : s_in)
        {
            string out = s;
                        
            for(size_t i = 0; i < out.size(); i++)
            {
                if(out[i] == '\\')
                {
                    out.replace(i, 1, "\\\\");
                    i++;
                }
                else if(out[i] == '\"')
                {
                    out.replace(i, 1, "\\\"");
                    i++;
                }
                // else if(i + 3 < out.size() && out[i + 1] == 'x')
                // {
                //     out.replace(i, 4, string(1, stoul(out.substr(i+2, 2), nullptr, 16)));
            }
            out.insert(out.begin(), '\"'); //leading quote
            out.insert(out.end() - 1, '\"'); //trailing quote
            s_out.push_back(out);
 
            full_count += out.size();
        }

        for(size_t i = 0; i < s_in.size(); i++)
            cout << s_in[i] << "(" << s_in[i].size() << "): "
                 << s_out[i] << "("<< s_out[i].size() << ")\n";

        cout << "b) added escapes literal - formatted = " << full_count - char_count << "\n";

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
