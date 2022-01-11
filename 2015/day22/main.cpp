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

bool operator==(const Spell a, const Spell b)
{
    return (a.name == b.name);
}

bool operator!=(const Spell a, const Spell b)
{
    return !(a == b);
}

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

vector<Spell> game_active_spells;
vector<Spell> game_spell_log;
Player game_player(0,0,0,0);
Player game_boss(0,0,0,0);
uint64_t game_end_count = 0;
uint64_t game_win_count = 0;
uint64_t game_mana_used = 0;
bool game_hard = false;

uint64_t best_mana_used = 0;
vector<Spell> best_spell_log;

void spell_effect(Spell spell)
{
    if(spell.damage)
        game_boss.hp -= spell.damage;
    if(spell.armor)
        game_player.armor += spell.armor;
    if(spell.hp_heal)
        game_player.hp += spell.hp_heal;
    if(spell.mana_heal)
        game_player.mana += spell.mana_heal;
}

void spells_effect()
{
    game_player.armor = 0; // reset to default before spells take effect
    size_t pos = 0;
    while(pos < game_active_spells.size())
    {
        auto& spell = game_active_spells[pos];

        spell_effect(game_active_spells[pos]);

        --spell.duration;
        if(spell.duration == 0)
        {
            game_active_spells.erase(game_active_spells.begin() + pos);
            continue; // don't increment
        }
        pos++;
    }
}

void game_win()
{
    game_win_count++;
    game_end_count++;

    if(game_mana_used < best_mana_used)
    {
        best_spell_log = game_spell_log;
        best_mana_used = game_mana_used;
    }
}

void next_round()
{
    if(game_hard)
    {
        if(--game_player.hp <= 0)
            return;
    }

    /* player turn */
    spells_effect();
    if(game_boss.hp <= 0)
    {
        game_win();
        return;
    }

    Player loop_top_player = game_player;
    Player loop_top_boss = game_boss;
    vector<Spell> loop_top_spell_log = game_spell_log;
    vector<Spell> loop_top_active_spells = game_active_spells;
    uint64_t loop_top_mana_used = game_mana_used;
    for(auto spell : spellbook)
    {
        auto it = find(game_active_spells.begin(), game_active_spells.end(), spell);
        if(game_active_spells.end() != it)
        {
            if(it->duration > 1)
                continue;
        }

        // spells are ordered low to high, so remaining will be too expensive also
        if( (game_mana_used + spell.mana_cost) >= best_mana_used)
            break;
        if(game_player.mana < spell.mana_cost)
            break;
            
        // Player casts spell
        game_spell_log.push_back(spell);
        game_player.mana -= spell.mana_cost;
        game_mana_used += spell.mana_cost;
        if(spell.duration == 1)
            spell_effect(spell);
        else
            game_active_spells.push_back(spell);

        /* boss turn */
        spells_effect();
        if(game_boss.hp <= 0)
        {
            game_win();
            // revert
            game_spell_log = loop_top_spell_log;
            game_player = loop_top_player;
            game_boss = loop_top_boss;
            game_active_spells = loop_top_active_spells;
            game_mana_used = loop_top_mana_used;
            continue;
        }
        // boss attacks
        int damage_to_player = (game_boss.damage - game_player.armor);
        if(damage_to_player < 1)
            damage_to_player = 1;
        game_player.hp -= damage_to_player;
        if(game_player.hp <= 0)
        {
            //game lose
            game_end_count++;
            // revert
            game_spell_log = loop_top_spell_log;
            game_player = loop_top_player;
            game_boss = loop_top_boss;
            game_active_spells = loop_top_active_spells;
            game_mana_used = loop_top_mana_used;
            continue;
        }

        next_round();

        // revert
        game_spell_log = loop_top_spell_log;
        game_player = loop_top_player;
        game_boss = loop_top_boss;
        game_active_spells = loop_top_active_spells;
        game_mana_used = loop_top_mana_used;
    }
}

pair<uint64_t,vector<Spell>> lowest_mana_win(Player player, Player boss, bool hard)
{
    game_spell_log.clear();
    game_active_spells.clear();
    game_player = player;
    game_boss = boss;
    game_mana_used = 0;
    best_mana_used = ~0ULL;
    best_spell_log.clear();
    game_end_count = 0;
    game_win_count = 0;
    game_hard = hard;

    next_round();

    return {best_mana_used, best_spell_log};
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
        auto result = lowest_mana_win(player_ex, boss_ex, false);
        if(result.second != vector<Spell>{sp_poison, sp_m_missle})
        {
            cout << flush;
            throw domain_error("example fails");
        }

        // example 1 - 2
        player_ex = {10, 0, 0, 250};
        boss_ex = {14, 8, 0, 0};
        result = lowest_mana_win(player_ex, boss_ex, false);
        if(result.second != vector<Spell>{sp_recharge, sp_shield, sp_drain, sp_poison, sp_m_missle})
        {
            cout << flush;
            throw domain_error("example fails");
        }

        result = lowest_mana_win({50,0,0,500}, {58,9,0,0}, false);
        cout << "a) lowest mana win: " << result.first << "\n";

        result = lowest_mana_win({50,0,0,500}, {58,9,0,0}, true);
        cout << "b) lowest mana win hard: " << result.first << "\n";

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
