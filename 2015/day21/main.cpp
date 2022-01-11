#include <bits/stdc++.h>
using namespace std;

// b 208 is incorrect

struct Item{
    Item(string name, int cost, int damage, int armor)
    : n{name}, c{cost}, d{damage}, a{armor}
    {

    }
    string n;
    int c;
    int d;
    int a;
};

ostream& operator<<(ostream& os, Item item)
{
    os << item.n << " cost:" << item.c
                 << " damage:" << item.d
                 << " armor:" << item.a << '\n';

    return os;
}

vector<Item> weapon{
    {"Dagger",       8, 4, 0},
    {"Shortsword",  10, 5, 0},
    {"Warhammer",   25, 6, 0},
    {"Longsword",   40, 7, 0},
    {"Greataxe",    74, 8, 0}    
};

vector<Item> armor{
    {"(naked)",     0, 0, 0},
    {"Leather",    13, 0, 1},
    {"Chainmail",  31, 0, 2},
    {"Splintmail", 53, 0, 3},
    {"Bandedmail", 75, 0, 4},
    {"Platemail", 102, 0, 5}    
};

vector<Item> ring{
    {"(finger)",    0, 0, 0},
    {"Damage+1",   25, 1, 0},
    {"Damage+2",   50, 2, 0},
    {"Damage+3",  100, 3, 0},
    {"Armor+1",    20, 0, 1},
    {"Armor+2",    40, 0, 2},
    {"Armor+3",    80, 0, 3},
};

struct Player{
    Player(int hp, int damage, int armor)
    : hp{hp}, damage{damage}, armor{armor}
    {

    }
    int hp;
    int damage;
    int armor;
};

pair<bool, int> play_game(Player player, Player boss)
{
    while(1)
    {
        int damage_to_boss = (player.damage - boss.armor);
        if(damage_to_boss < 1)
            damage_to_boss = 1;
        boss.hp -= damage_to_boss;
        if(boss.hp <= 0)
            return {true, player.hp};

        int damage_to_player = (boss.damage - player.armor);
        if(damage_to_player < 1)
            damage_to_player = 1;
        player.hp -= damage_to_player;
        if(player.hp <= 0)
            return {false, boss.hp};
    }

    return {0, 999}; //error
}

pair<uint64_t, vector<Item>> lowest_cost_win(int player_hp, Player boss)
{
    uint64_t cost = 0;    
    vector<Item> equip;

    uint64_t best_cost = ~0ULL;
    vector<Item> best_equip;

    for(auto w : weapon)
    {
        cost += w.c;
        equip.push_back(w);
        for(auto a : armor)
        {
            if((cost + a.c) > best_cost)
                continue;
            cost += a.c;
            equip.push_back(a);
            for(auto lr : ring)
            {
                if((cost + lr.c) > best_cost)
                    continue;
                cost += lr.c;
                equip.push_back(lr);
                    for(auto rr : ring)
                    {
                        if((cost + rr.c) > best_cost)
                            continue;
                        cost += rr.c;
                        equip.push_back(rr);

                        int damage = 0;
                        int armor = 0;
                        for(auto e : equip)
                        {
                            damage += e.d;
                            armor += e.a;
                        }

                        auto result = play_game(Player(player_hp, damage, armor), boss);
                        if(result.first)
                        {
                            best_cost = cost;
                            best_equip = equip;
                        }

                        equip.pop_back();
                        cost -= rr.c;
                    }
                equip.pop_back();
                cost -= lr.c;
            }
            equip.pop_back();
            cost -= a.c;
        }
        equip.pop_back();
        cost -= w.c;
    }

    return {best_cost, best_equip};
}

pair<uint64_t, vector<Item>> highest_cost_loss(int player_hp, Player boss)
{
    uint64_t cost = 0;    
    vector<Item> equip;

    uint64_t best_cost = 0;
    vector<Item> best_equip;

    for(auto w : weapon)
    {
        cost += w.c;
        equip.push_back(w);
        for(auto a : armor)
        {
            cost += a.c;
            equip.push_back(a);
            for(auto lr : ring)
            {
                cost += lr.c;
                equip.push_back(lr);
                    for(auto rr : ring)
                    {
                        if(rr.n != "(finger" && rr.n == lr.n)
                            continue;
                        if((cost + rr.c) < best_cost)
                            continue;
                        cost += rr.c;
                        equip.push_back(rr);

                        int damage = 0;
                        int armor = 0;
                        for(auto e : equip)
                        {
                            damage += e.d;
                            armor += e.a;
                        }

                        auto result = play_game(Player(player_hp, damage, armor), boss);
                        if(!result.first)
                        {
                            best_cost = cost;
                            best_equip = equip;
                        }

                        equip.pop_back();
                        cost -= rr.c;
                    }
                equip.pop_back();
                cost -= lr.c;
            }
            equip.pop_back();
            cost -= a.c;
        }
        equip.pop_back();
        cost -= w.c;
    }

    return {best_cost, best_equip};
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

        Player boss_ex(12, 7, 2);
        Player player_ex(8, 5, 5);
        if(pair<bool, int>({true, 2}) != play_game(player_ex, boss_ex))
            throw domain_error("example fails");

        Player boss(103, 9, 2);
        auto result = lowest_cost_win(100, boss);
        cout << "a) lowest cost win: " << result.first << "\n";
        for(auto e : result.second)
            cout << "  " << e.n << "\n";
        cout << '\n';

        result = highest_cost_loss(100, boss);
        cout << "a) highest cost loss: " << result.first << "\n";
        for(auto e : result.second)
            cout << "  " << e.n << "\n";

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
