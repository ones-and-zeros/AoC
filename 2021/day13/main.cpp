#include <bits/stdc++.h>
#include <cctype>
using namespace std;

using point = pair<int,int>;
using fold = pair<char,int>;

set<point> points;
int x_qty = 0;
int y_qty = 0;

vector<fold> folds;

vector<int> points_after_fold;

void print_grid()
{
    for(int y = 0; y < y_qty; y++)
    {
        for(int x = 0; x < x_qty; x++)
        {
            if(points.find({x,y}) != points.end())
                cout << "#";
            else
                cout << ".";
        }
        cout << endl;
    }
}

void fold_this(fold f)
{
    set<point> to_remove;
    set<point> to_add;

    if(f.first == 'y')
    {
        y_qty = f.second;

        for(auto& p : points)
        {
            if(p.second > y_qty)
            {
                to_remove.insert(p);
                to_add.insert({p.first, y_qty - (p.second - y_qty)});
            }
        }
    }
    else if(f.first == 'x')
    {
        x_qty = f.second;

        for(auto& p : points)
        {
            if(p.first > x_qty)
            {
                to_remove.insert(p);
                to_add.insert({x_qty - (p.first - x_qty), p.second});
            }
        }
    }

    for(auto p : to_remove)
        points.erase(p);

    for(auto p : to_add)
        points.insert(p);
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string file_line;

        //parse lines
        while(getline(infile, file_line))
        {
            if(isdigit(file_line[0]))
            {
                istringstream iss{file_line};
                string s1, s2;
                getline(iss, s1, ',');
                getline(iss, s2);
                int x = stoi(s1);
                int y = stoi(s2);
                points.insert({x,y});
                if(x_qty <= x)
                    x_qty = x+1;
                if(y_qty <= y)
                    y_qty = y+1;
            }
            else
            {
                istringstream iss{file_line};
                string s;
                getline(iss, s, '=');
                if(s == "fold along x")
                {
                    getline(iss, s);
                    folds.push_back({'x',stoi(s)});
                }
                else if(s == "fold along y")
                {
                    getline(iss, s);
                    folds.push_back({'y',stoi(s)});
                }
            }
        }

        cout << "grid size: " << x_qty << "x" << y_qty << endl;
        cout << endl;

        // cout << "points:" << endl;
        // for(auto p : points)
        //     cout << " {" << p.first << " ," << p.second << "}" << endl;
        // cout << endl;

        // cout << endl << "orig:" << endl;
        // print_grid();
        // cout << endl;

        // cout << "folds: " << endl;
        // for(auto f : folds)
        //     cout << "fold " << f.first << " at " << f.second << endl;
        // cout << endl;

        for(auto f : folds)
        {
            fold_this(f);
            points_after_fold.push_back(points.size());

            // cout << "fold " << f.first << " at " << f.second << endl;
            // print_grid();
            // cout << endl;
        }


        cout << "a - dots visible after 1st fold: " << points_after_fold[0] << endl;
        cout << endl;

        cout << "b - decoded image:" << endl;
        print_grid();


        infile.close();
    }

    // execution time
    auto stop = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
    cout << "sec: " << (duration.count() / 1000000) << 
                    "." <<  setfill('0') << setw(6) << (duration.count() % 1000000) << endl;
    cout << endl;
}
