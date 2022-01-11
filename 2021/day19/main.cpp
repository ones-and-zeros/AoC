#include <bits/stdc++.h>
using namespace std;

// a) 607 and 625 too high

constexpr size_t x_point = 0;
constexpr size_t y_point = 1;             
constexpr size_t z_point = 2;

using Point = vector<int>;
using Report = vector<Point>;
using Table = vector<Report>;

constexpr size_t min_match_count(12);

// 6 ways axis can align (6 sides of dice)
vector<pair<Point,Point>> axis_align_pol{
    { {0,1,2},  { 1, 1, 1} }, //  x  y  z
    { {0,1,2},  { 1,-1,-1} }, //  x -y -z
    { {0,1,2},  {-1,-1, 1} }, // -x -y  z
    { {0,1,2},  {-1, 1,-1} }, // -x  y -z

    { {0,2,1},  {-1,-1,-1} }, // -x -z -y
    { {0,2,1},  {-1, 1, 1} }, // -x  z  y
    { {0,2,1},  { 1, 1,-1} }, //  x -z  y
    { {0,2,1},  { 1,-1, 1} }, //  x  z -y

    { {1,0,2},  {-1,-1,-1} }, // -y -x -z
    { {1,0,2},  {-1, 1, 1} }, //  y -x  z
    { {1,0,2},  { 1, 1,-1} }, //  y  x -z
    { {1,0,2},  { 1,-1, 1} }, // -y  x  z

    { {1,2,0},  { 1, 1, 1} }, //  y  z  x
    { {1,2,0},  { 1,-1,-1} }, // -y -z  x
    { {1,2,0},  {-1,-1, 1} }, // -y  z -x
    { {1,2,0},  {-1, 1,-1} }, //  y -z -x

    { {2,0,1},  { 1, 1, 1} }, //  z  x  y
    { {2,0,1},  { 1,-1,-1} }, // -z  x -y
    { {2,0,1},  {-1,-1, 1} }, //  z -x -y
    { {2,0,1},  {-1, 1,-1} }, // -z -x  y

    { {2,1,0},  {-1,-1,-1} }, // -z -y -x
    { {2,1,0},  {-1, 1, 1} }, //  z  y -x
    { {2,1,0},  { 1, 1,-1} }, // -z  y  x
    { {2,1,0},  { 1,-1, 1} }, //  z -y  x
};

Point adjust(const Point& org, const vector<int>& al, const vector<int>& pol, const Point& offset)
{
    return { org[al[x_point]] * pol[x_point] + offset[x_point],
             org[al[y_point]] * pol[y_point] + offset[y_point],
             org[al[z_point]] * pol[z_point] + offset[z_point] };
}

Point negat(const Point& org)
{
    return { -org[x_point], -org[y_point], -org[z_point] };
}

bool equa(const Point& a, const Point& b)
{
    return (a[0]==b[0] && a[1]==b[1] && a[2]==b[2]);
}

ostream& operator<<(ostream& os, Point pt)
{
    os << "{" << setw(5) << pt[0] << ", " << setw(5) << pt[1] << ", " << setw(5) << pt[2] << "}";
    return os;
}

int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Report> scanners;

        //parse lines
        while(getline(infile, line))
        {            
            if(line.empty())
                break;
            if(line.substr(0,8) != "--- scan")
                throw range_error("input error");

            Report report;        
            while(getline(infile, line))
            {
                if(line.empty())
                    break;
                istringstream iss{line};
                string x_s, y_s, z_s;
                getline(iss, x_s, ',');
                getline(iss, y_s, ',');
                getline(iss, z_s);
                report.push_back( {stoi(x_s), stoi(y_s), stoi(z_s)} );
            }
            scanners.push_back(report);
        }

        cout << endl;

        // create table of reports,
        // each report uses one (of each) unique point as reference
        vector<Table> scan_rel;
        vector<Table> scan_rel_abs;
        for(auto sc : scanners)
        {
            vector<Report> table; 
            vector<Report> table_abs; 

            for(size_t i = 0; i < sc.size(); i++)
            {
                Point os = { 0 - sc[i][x_point], 0 - sc[i][y_point], 0 - sc[i][z_point]};
                Report report;
                Report report_abs;
                for(auto p : sc)
                {
                    p = { p[x_point] + os[x_point], p[y_point] + os[y_point], p[z_point] + os[z_point] };
                    report.push_back(p);
                    report_abs.push_back({abs(p[x_point]), abs(p[y_point]), abs(p[z_point])});
                }
                table.push_back(report);
                table_abs.push_back(report_abs);
            }
            scan_rel.push_back(table);
            scan_rel_abs.push_back(table_abs);
        }

        vector<pair<Point,Point>> scanner_align(scanners.size(), {{0,1,2}, {1,1,1}});
        vector<Point> scanner_offset(scanners.size(), {0,0,0});

        unsigned long long completed = 1; //bitwise, scanner 1 is ref, so it is completed
        unsigned long long total = 0; //bitwise        
        set<pair<size_t,size_t>> attempted;
        for(size_t i = 0; i < scanners.size(); i++)
            total |= 1ULL << i;

        while(completed != total)
        {
            for(size_t known = 0; known < scanners.size(); known++)
            {
                if(!(completed & (1ULL << known)))
                    continue; // not known yet

                for(size_t un = 0; un < scanners.size(); un++)
                {
                    if(completed & (1ULL << un))
                        continue; // known already

                    /* only attempt each valid pair once */
                    if(attempted.end() != attempted.find({known,un}))
                        continue;
                    attempted.insert({known,un});

                    for(auto al : axis_align_pol)
                    {
                        for(size_t ua_i = 0; ua_i < scan_rel[un].size(); ua_i++)
                        {
                            for(size_t ka_i = 0; ka_i < scan_rel[known].size(); ka_i++)
                            {
                                size_t tally = 0;
                                size_t remain = scan_rel[un][ua_i].size();
                                for(auto up : scan_rel[un][ua_i])
                                {
                                    //check for axis-adjusted point
                                    Point adj = adjust(up, al.first, al.second, {0,0,0});
                                    auto result = find(scan_rel[known][ka_i].begin(), scan_rel[known][ka_i].end(), adj);
                                    if(scan_rel[known][ka_i].end() != result)
                                    {
                                        tally++;
                                        // if(tally > 1) // you always get one {0,0,0} match, so skip showing those
                                        //     cout << setw(3) << tally << " match " << un << " r" << known << ": " << *result << " = " << up << endl;
                                    }
                                    remain--;
                                    if((tally + remain) < min_match_count)
                                        break;
                                }
                                if(tally >= min_match_count)
                                {
                                    // alignment found !!!
                                    // calibrate...

                                    //find zero points
                                    size_t kzp;
                                    for(kzp = 0; kzp < scanners[known].size(); kzp++)
                                        if(equa(scan_rel[known][ka_i][kzp], {0,0,0})) 
                                            break;
                                    size_t uzp;
                                    for(uzp = 0; uzp < scanners[un].size(); uzp++)
                                        if(equa(scan_rel[un][ua_i][uzp], {0,0,0}))
                                            break;

                                    /* calculate cal points */
                                    scanner_align[un].first = adjust( al.first, scanner_align[known].first, {1,1,1}, {0,0,0});
                                    scanner_align[un].second = adjust( al.second, scanner_align[known].first, scanner_align[known].second, {0,0,0});

//                                        kf(kzp) - kf(-uf(uzp))
                                    Point ref_to_known = adjust(negat(scanners[un][uzp]), al.first, al.second, scanners[known][kzp]);
                                    scanner_offset[un] = adjust(ref_to_known, scanner_align[known].first, scanner_align[known].second, scanner_offset[known]);

                                    cout << "scanner " << un << " found (ref " << known << "): \n";
                                    cout << "  a match : " << scanners[un][0] << " = " << adjust(scanners[un][0], scanner_align[un].first, scanner_align[un].second, scanner_offset[un]) << endl;
                                    cout << "  align   : " << scanner_align[un].first << endl;
                                    cout << "  polarity: " << scanner_align[un].second << endl;
                                    cout << "  position: " << scanner_offset[un] << endl;
                                    cout << endl;

                                    // what was unknown is now known (completed)
                                    completed |= (1ULL << un);
                                    // break out of un loop
                                    goto bottom_un_loop;
                                }
                            }
                        }
                    }
bottom_un_loop:
                    asm("nop");
                }
            }
        }

        set<Point> master;
        for(size_t i = 0; i < scanners.size(); i++)
            for(auto pt : scanners[i])
                master.insert( adjust(pt, scanner_align[i].first, scanner_align[i].second, scanner_offset[i]) );

        cout << endl;
        cout << " total " << master.size() << ": " << endl;
        for(auto pt : master)
        {
            cout << pt << "\n";
        }
        cout << " a - total " << master.size() << ": " << endl;

        unsigned long long largest_manhatten = 0;
        for(size_t i = 0; i < scanners.size(); i++)
        {
            for(size_t j = i + 1; j < scanners.size(); j++)
            {                
                unsigned long long md = 0;
                md += abs(scanner_offset[i][0] - scanner_offset[j][0]);
                md += abs(scanner_offset[i][1] - scanner_offset[j][1]);
                md += abs(scanner_offset[i][2] - scanner_offset[j][2]);
                if(md > largest_manhatten)
                    largest_manhatten = md;
            }
        }
        cout << " b - largest manhatten distance " << largest_manhatten << endl;

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
