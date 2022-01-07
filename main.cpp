#include <bits/stdc++.h>
using namespace std;

struct Spell{
    string name;
    int mana_cost;
    int duration;
    int damage;
    int armor;
    int hp_heal;
    int mana_heal;
};

                        //name           cst dr dm ar hl man
const Spell sp_m_missle {"Magic Missle", 53, 1, 4, 0, 0,   0};
const Spell sp_drain    {"Drain",        73, 1, 2, 0, 2,   0};
const Spell sp_shield   {"Shield",      113, 6, 0, 7, 0,   0};
const Spell sp_poison   {"Poison",      173, 6, 3, 0, 0,   0};
const Spell sp_recharge {"Recharge",    229, 5, 0, 0, 0, 101};

vector<Spell> spellbook{
    sp_m_missle,
    sp_drain,
    sp_shield,
    sp_poison,
    sp_recharge
};

struct Player{
    Player(int hp, int damage, int armor, int mana)
    : hp{hp}, damage{damage}, armor{armor}, mana{mana}
    {

    }
    int hp;
    int damage;
    int armor;
    int mana;
};


void print_score(const Player player, const Player boss)
{
    cout << "- Player has " << player.hp << " hit points, " <<
            player.armor << " armor, " << player.mana << " mana\n";
    cout << "- Boss has " << boss.hp << " hit points\n";
}

bool operator==(const Player a, const Player b)
{
    return ( a.armor    == b.armor &&
             a.damage   == b.damage &&
             a.hp       == b.hp &&
             a.mana     == b.mana);
}

bool operator!=(const Player a, const Player b)
{
    return !(a==b);
}

void spell_effect(Player& player, Player& boss, vector<Spell>& spells)
{
    player.armor = 0;
    size_t pos = 0;
    while(pos < spells.size())
    {
        auto& spell = spells[pos];

        cout << spell.name << " effect:";
        if(spell.damage)
        {
            cout << "  boss hp -" << spell.damage; 
            boss.hp -= spell.damage;
        }
        if(spell.armor)
        {
            cout << "  arm +" << spell.armor; 
            player.armor += spell.armor;
        }
        if(spell.hp_heal)
        {
            cout << "  hp +" << spell.hp_heal; 
            player.hp += spell.hp_heal;
        }
        if(spell.mana_heal)
        {
            cout << "  mana +" << spell.mana_heal; 
            player.mana += spell.mana_heal;
        }
        
        --spell.duration;
        cout << "  timer " << spell.duration << "\n";
        if(spell.duration == 0)
        {
            cout << spell.name << " expires\n";
            spells.erase(spells.begin() + pos);
            continue; // don't increment
        }
        pos++;
    }
}

pair<bool, Player> play_game(Player player, Player boss, vector<Spell> spellorder)
{
    vector<Spell> active_spells;

    for(auto cast : spellorder)
    {
        /* player turn */
        cout << "\n-- Player turn --\n";
        print_score(player, boss);
        // spells have effect
        spell_effect(player, boss, active_spells);
        if(boss.hp <= 0)
        {
            cout << "Player wins!\n";
            return {true, player};
        }
        // Player casts spell
        player.mana -= cast.mana_cost;
        if(player.mana < 0)
        {
            cout << "Player loses, out of mana\n";
            return {false, boss}; //todo better indication ran out of mana
        }
        cout << "Player casts " << cast.name << "\n";
        if(cast.duration == 1)
        {
            //apply now
            vector<Spell> casted{cast};
            spell_effect(player, boss, casted);
        }
        else
            active_spells.push_back(cast);

        /* boss turn */
        cout << "\n-- Boss turn --\n";
        print_score(player, boss);
        // spells have effect
        spell_effect(player, boss, active_spells);
        if(boss.hp <= 0)
        {
            cout << "Player wins!\n";
            return {true, player};
        }
        // boss attacks
        int damage_to_player = (boss.damage - player.armor);
        if(damage_to_player < 1)
            damage_to_player = 1;
        cout << "Boss attacks, damage is " << damage_to_player << "\n";
        player.hp -= damage_to_player;
        if(player.hp <= 0)
        {
            cout << "Player loses\n";
            return {false, boss};
        }
    }

    cout << flush;
    throw domain_error("ran out of spells");

    return {0, {999,999,999,999}}; //error
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

        // example 1 - 1
        Player player_ex(10, 0, 0, 250);
        Player boss_ex(13, 8, 0, 0);
        vector<Spell> spell_que({sp_poison, sp_m_missle});
        if(pair<bool,Player>({true, Player{2,0,0,24}}) != play_game(player_ex, boss_ex, spell_que))
        {
            cout << flush;
            throw domain_error("example fails");
        }

        // example 1 - 2
        player_ex = {10, 0, 0, 250};
        boss_ex = {14, 8, 0, 0};
        spell_que = vector<Spell>{sp_recharge, sp_shield, sp_drain, sp_poison, sp_m_missle};
        if(pair<bool,Player>({true, Player{1,0,0,114}}) != play_game(player_ex, boss_ex, spell_que))
        {
            cout << flush;
            throw domain_error("example fails");
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
