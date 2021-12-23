#include <bits/stdc++.h>
using namespace std;

constexpr size_t x_point = 0;
constexpr size_t y_point = 1;
constexpr size_t z_point = 2;

using Point = vector<int>;

using Report = vector<Point>;
using Table = vector<Report>;

constexpr size_t min_match_count(12);

// 6 ways axis can align (6 sides of dice)
vector<vector<int>> axis_align{
    {0,1,2},
    {0,2,1},
    {1,0,2},
    {1,2,0},
    {2,0,1},
    {2,1,0} };

// 4 directions a side can face, and the effect on the axis polarity
vector<vector<int>> axis_polarity{ { 1, 1, 1},
                                   {-1,-1, 1},
                                   {-1, 1,-1},
                                   { 1,-1,-1}};

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


        // for(size_t i = 0; i < scanners.size(); i++)
        // {
        //     cout << "scan rel " << i << ": " << endl;

        //     for(auto p : scan_rel[i][0])
        //        cout << " { " <<  p[0] << ", " << p[1] << ", " << p[2] << " }" << endl;
        //     cout << endl;
        // }
        // cout << endl;

        // for(size_t i = 0; i < scanners.size(); i++)
        // {
        //     cout << "scan rel abs" << i << ": " << endl;

        //     for(auto p : scan_rel_abs[i][0])
        //        cout << " { " <<  p[0] << ", " << p[1] << ", " << p[2] << " }" << endl;
        //     cout << endl;
        // }

        // for(size_t i = 0; i < axis_align.size(); i++)
        //     cout << axis_align[i][0] << " " << axis_align[i][1] << " " << axis_align[i][2] << "\n";
        // cout << endl;

        // for(size_t i = 0; i < axis_polarity.size(); i++)
        //     cout << setw(2) << axis_polarity[i][0] << " " << setw(2) << axis_polarity[i][1] << " " << setw(2) << axis_polarity[i][2] << "\n";
        // cout << endl;




        vector<vector<int>> scanner_align(scanners.size(), {0,1,2});
        vector<vector<int>> scanner_pol(scanners.size(), {1,1,1});
        vector<Point> scanner_offset(scanners.size(), {0,0,0});

        size_t completed = 1; //bitwise, scanner 1 is ref, so it is completed
        size_t total = 0; //bitwise        
        set<pair<size_t,size_t>> attempted;
        for(size_t i = 0; i < scanners.size(); i++)
            total |= 1U << i;

        while(completed != total)
        {
            for(size_t known = 0; known < scanners.size(); known++)
            {
                if(!(completed & (1U << known)))
                    continue; // not known yet

                for(size_t un = 0; un < scanners.size(); un++)
                {
                    if(completed & (1U << un))
                        continue; // known already

                    /* only attempt each valid pair once */
                    if(attempted.end() != attempted.find({known,un}))
                        continue;
                    attempted.insert({known,un});








                    /* cycle through the 6 axis alignments */
                    for(auto al : axis_align)
                    {
                        for(size_t ua_i = 0; ua_i < scan_rel_abs[un].size(); ua_i++)
                        {
                            for(size_t ka_i = 0; ka_i < scan_rel_abs[known].size(); ka_i++)
                            {
                                size_t tally = 0;
                                size_t remain = scan_rel_abs[un][ua_i].size();
                                for(auto up : scan_rel_abs[un][ua_i])
                                {
                                    //check for axis-adjusted point
                                    Point adj{up[al[x_point]], up[al[y_point]], up[al[z_point]]};
                                    if(scan_rel_abs[known][ka_i].end() != find(scan_rel_abs[known][ka_i].begin(), scan_rel_abs[known][ka_i].end(), adj))
                                        tally++;
                                    remain--;
                                    if((tally + remain) < min_match_count)
                                        break;
                                }
                                if(tally >= min_match_count)
                                {
                                    /* at this point
                                        scan_rel_abs[un][ua_i] and
                                        scan_rel_abs[known][ka_i] have been tentatively matched,
                                        but polarity has not been checked yet */
                                    
                                    /* cycle through the 4 axis polarity */
                                    for(auto ap : axis_polarity)
                                    {
                                        tally = 0;
                                        for(auto upr : scan_rel[un][ua_i])
                                        {
                                            Point adj{ upr[al[x_point]] * ap[x_point],
                                                       upr[al[y_point]] * ap[y_point],
                                                       upr[al[z_point]] * ap[z_point]};
                                            auto result = find(scan_rel[known][ka_i].begin(), scan_rel[known][ka_i].end(), adj);
                                            if(result != scan_rel[known][ka_i].end())
                                            {
                                                tally++;
                                                cout << "Match found " << tally << ": { " << (*result)[0] << ", " << (*result)[1] << ", " << (*result)[2] <<
                                                            " } = { " << upr[0] << ", " << upr[1] << ", " << upr[2] << " } " << endl;
                                            }
                                        }
                                        if(tally < min_match_count)
                                            continue;


                                        // alignment found <--------------- !!!
                                        // todo set offset vals

                                        // scanner_align[un] = {
                                        //     al[scanner_align[known][x_point]],
                                        //     al[scanner_align[known][y_point]],
                                        //     al[scanner_align[known][z_point]] };
                                        scanner_align[un] = al;
                                        
                                        // scanner_pol[un] = {
                                        //     ap[x_point] * scanner_pol[known][x_point],
                                        //     ap[y_point] * scanner_pol[known][y_point],
                                        //     ap[z_point] * scanner_pol[known][z_point] };
                                        scanner_pol[un] = ap;


                                        //find zero points
                                        size_t kzp;
                                        for(kzp = 0; kzp < scanners[known].size(); kzp++)
                                        {
                                            if( (scan_rel_abs[known][ka_i][kzp][x_point] == 0) &&
                                                (scan_rel_abs[known][ka_i][kzp][y_point] == 0) &&
                                                (scan_rel_abs[known][ka_i][kzp][z_point] == 0) )
                                            {
                                                break;
                                            }
                                        }
                                        size_t uzp;
                                        for(uzp = 0; uzp < scanners[un].size(); uzp++)
                                        {
                                            if( (scan_rel_abs[un][ua_i][uzp][x_point] == 0) &&
                                                (scan_rel_abs[un][ua_i][uzp][y_point] == 0) &&
                                                (scan_rel_abs[un][ua_i][uzp][z_point] == 0) )
                                            {
                                                break;
                                            }
                                        }

                                        scanner_offset[un] = { scanners[known][kzp][x_point],
                                                               scanners[known][kzp][y_point],
                                                               scanners[known][kzp][z_point] };
                                        
                                        scanner_offset[un][x_point] -= scanners[un][uzp][scanner_align[un][x_point]] * scanner_pol[un][x_point];
                                        scanner_offset[un][y_point] -= scanners[un][uzp][scanner_align[un][y_point]] * scanner_pol[un][y_point];
                                        scanner_offset[un][z_point] -= scanners[un][uzp][scanner_align[un][z_point]] * scanner_pol[un][z_point];





                                        cout << endl;
                                        cout << "scanner " << un << " found: \n";
                                        cout << "  align: " << scanner_align[un][0] << " " <<
                                                               scanner_align[un][1] << " " <<
                                                               scanner_align[un][2] << "\n";

                                        cout << "  polarity: " << scanner_pol[un][0] << " " <<
                                                                  scanner_pol[un][1] << " " <<
                                                                  scanner_pol[un][2] << "\n";
                                        cout << "  position: " << scanner_offset[un][0] << " " <<
                                                                  scanner_offset[un][1] << " " <<
                                                                  scanner_offset[un][2] << "\n";
                                                               
                                        cout << endl;



                                        cout << "found " << un << ": " << endl;
                                        for(auto pt : scanners[un])
                                        {
                                            Point adj{ scanner_offset[un][x_point] + pt[scanner_align[un][x_point]] * scanner_pol[un][x_point],
                                                       scanner_offset[un][y_point] + pt[scanner_align[un][y_point]] * scanner_pol[un][y_point],
                                                       scanner_offset[un][z_point] + pt[scanner_align[un][z_point]] * scanner_pol[un][z_point] };

                                            cout << "  { " << pt[0] << ", " << pt[1] << ", " << pt[2] <<
                                                            " } = { " << adj[0] << ", " << adj[1] << ", " << adj[2] << " } " << endl;
                                            
                                            
                                        }

                                        // cout << "found:\n";
                                        // for(auto p : scanners[un])
                                        // {
                                        //     Point adj{ p[al[x_point]] * ap[x_point],
                                        //                 p[al[y_point]] * ap[y_point],
                                        //                 p[al[z_point]] * ap[z_point]};

                                        //     auto result = find(scanners[known].begin(), scanners[known].end(), adj);
                                        //     if(result != scanners[known].end())
                                        //     {
                                        //         cout << "  { " << (*result)[0] << ", " << (*result)[1] << ", " << (*result)[2] <<
                                        //                     " } = { " << p[0] << ", " << p[1] << ", " << p[2] << " } " << endl;
                                        //         tally++;
                                        //     }
                                        // }





                                        asm("nop");



                                        // what was unknown is now known (completed)
                                        completed |= 1 << un;
                                        // break out of un loop
                                        goto bottom_un_loop;
                                    }
                                }
                            }
                        }
                    }
bottom_un_loop:
                    asm("nop");
                }
            }
        }



        // for(size_t i = 0; i < axis_align.size(); i++)
        //     cout << axis_align[i][0] << " " << axis_align[i][1] << " " << axis_align[i][2] << "\n";
        // cout << endl;

        // for(size_t i = 0; i < axis_polarity.size(); i++)
        //     cout << setw(2) << axis_polarity[i][0] << " " << setw(2) << axis_polarity[i][1] << " " << setw(2) << axis_polarity[i][2] << "\n";
        // cout << endl;

        asm("nop");


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
