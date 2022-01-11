#include <bits/stdc++.h>
using namespace std;

using Sf_expr = string;

bool explode(Sf_expr& expr)
{
    int depth = 0;
    size_t left_pos = 0;

    for(size_t i = 0; i < expr.size(); i++)
    {
        if(isdigit(expr[i]))
            if(!isdigit(expr[i - 1]))
                left_pos = i;

        if(expr[i] == '[')
            depth++;
        if(expr[i] == ']')
            depth--;

        if(depth < 5)
            continue;

        /* time to explode */

        string value_s;
        int lp_i = 0;
        int rp_i = 0;

        //erase leading bracket
        expr.erase(i, 1);

        //get left pair intiger
        while(isdigit(expr[i]))
        {
            value_s += expr[i];
            expr.erase(i, 1); //erase as processed
        }
        lp_i = stoi(value_s);
        // erase comma
        expr.erase(i, 1); 
        // get right pair int
        value_s.clear();
        while(isdigit(expr[i]))
        {
            value_s += expr[i];
            expr.erase(i, 1); //erase as processed
        }
        rp_i = stoi(value_s);
        // erase closin bracket
        expr.erase(i, 1); 

        // replace exploded pair with 0
        expr.insert(i, "0");

        // look for next number to right
        value_s.clear();
        for(size_t j = i + 1; j < expr.size(); j++)
        {
            if(isdigit(expr[j]))
            {
                value_s += expr[j];
                continue;
            }
            else if(value_s.empty())
                continue;

            //adjust j back to start of digit
            j--;
            while(isdigit(expr[j-1]))
                j--;

            while(isdigit(expr[j]))
                expr.erase(j, 1);

            expr.insert(j, to_string((stoi(value_s) + rp_i)));
            break;
        }

        // check if left number present
        if(left_pos > 0)
        {
            value_s.clear();
            while(isdigit(expr[left_pos]))
            {
                value_s += expr[left_pos];
                expr.erase(left_pos, 1);
            }

            expr.insert(left_pos, to_string((stoi(value_s) + lp_i)));
        }

        asm("nop");


        return true;
    }

    return false;
}

bool split(Sf_expr& expr)
{
    for(size_t i = 0; i < (expr.size() - 1); i++)
    {
        if(!isdigit(expr[i]))
            continue;
        if(!isdigit(expr[i+1]))
            continue;

        /* number with 2+ digits found, need to split */

        string val_s;
        while(isdigit(expr[i]))
        {
            val_s += expr[i];
            expr.erase(i, 1);
        }
        int val = stoi(val_s);

        expr.insert(i, ("[" + to_string(val/2) + "," + to_string((val+1)/2) + "]"));

        return true;
    }

    return false;
}

void simplify(Sf_expr& expr)
{
    while(1)
    {
//        cout << expr << endl;

        if(explode(expr))
            continue;
        if(split(expr))
            continue;
        break;
    }
}

Sf_expr add(const Sf_expr& base, const Sf_expr& to_add)
{
//    cout << base << " plus " << to_add << endl;

    Sf_expr result = "[" + base + "," + to_add + "]";
    simplify(result);
    return result;
}

unsigned long mag_(Sf_expr& expr)
{
    unsigned long val = 0;
    if(isdigit(expr[0]))
    {
        val = stoi(expr);
        expr.erase(0,1); //erase digit
        return val;
    }

    if(expr[0] != '[')
        throw range_error("Missing '['");
    expr.erase(0,1); //erase char

    // 3 times the magnitude of its left element
    val = mag_(expr) * 3;

    if(expr[0] != ',')
        throw range_error("Missing ','");
    expr.erase(0,1); //erase char

    // 2 times the magnitude of its right element
    val += mag_(expr) * 2;

    if(expr[0] != ']')
        throw range_error("Missing ']'");
    expr.erase(0,1); //erase char

    return val;
}

unsigned long magnitude(Sf_expr expr)
{
    //jic
    simplify(expr);
    
    return mag_(expr);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Sf_expr> exprs;

        //parse lines
        while(getline(infile, line))
        {
            exprs.push_back(line);
            // istringstream iss{line};
            // string x_min, x_max, y_min, y_max, junk;
            // getline(iss, junk, '=');
            // getline(iss, x_min, '.');

        }

        cout << endl;

        Sf_expr result = exprs[0];
        for(size_t i = 1; i < exprs.size(); i++)
            result = add(result, exprs[i]);
        cout << endl;

        cout << "a - final mag: " << magnitude(result) << endl;

        vector<unsigned long> mags;
        for(int i = 0; i < exprs.size(); i++)
            for(int j = i + 1; j < exprs.size(); j++)
                mags.push_back(magnitude(add(exprs[i],exprs[j])));                
        sort(mags.begin(), mags.end());
        cout << "b - largest mag: " << mags[mags.size()-1] << endl;


        cout << endl;
        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
