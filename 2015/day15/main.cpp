#include <bits/stdc++.h>
using namespace std;

constexpr size_t attribute_qty{5};
constexpr size_t at_cap{0};
constexpr size_t at_dur{1};
constexpr size_t at_flv{2};
constexpr size_t at_txt{3};
constexpr size_t at_cal{4};

enum class Attribute {
    capacity = 0,
    durability,
    flavor,
    texture,
    calories
};

struct Ingredient{
    Ingredient(
        string name,
        int capacity,
        int durability,
        int flavor,
        int texture,
        int calories)
    : name{name},
      attributes{ capacity,
                  durability,
                  flavor,
                  texture,
                  calories }
    {
        
    }
    string name;
    vector<int> attributes;
};

uint64_t best_score = 0;
vector<uint32_t> best_recipe;

vector<Ingredient> master_ingredients;
vector<uint32_t> recipe;
uint64_t remaining_units = 0;

bool calories_matter = false;
constexpr uint32_t calorie_target{500};

void next_ingr(size_t depth)
{
    if(depth + 1 == master_ingredients.size())
    {
        // last ingredient, so use remaining space allotted
        recipe[depth] = remaining_units;

        if(calories_matter)
        {
            uint64_t calories = 0;
            for(size_t i = 0; i < master_ingredients.size(); i++)
            {
                calories += recipe[i] * static_cast<int64_t>(master_ingredients[i].attributes[at_cal]);
            }
            if(calories != calorie_target)
                return;
        }

        size_t score = 1;
        for(size_t a = at_cap; a <= at_txt; a++)
        {
            int64_t sub_score = 0;
            for(size_t i = 0; i < master_ingredients.size(); i++)
            {
                sub_score += recipe[i] * static_cast<int64_t>(master_ingredients[i].attributes[a]);
            }
            if(sub_score <= 0)
                return; // no points, negative or zero attribute

            score *= sub_score;
        }
        if(best_score <= score)
        {
            best_score = score;
            best_recipe = recipe;
        }
        return;
    }

    for(uint32_t qty = 0; qty <= remaining_units; qty++)
    {
        remaining_units -= qty;
        recipe[depth] = qty;
        next_ingr(depth + 1);
//        recipe[depth] = 0;
        remaining_units += qty;
    }
}

uint64_t create_recipe(vector<Ingredient> ingredients, size_t total_units, bool count_calories)
{
    master_ingredients = ingredients;
    recipe = vector<uint32_t>(ingredients.size(), {0});
    remaining_units = total_units;
    best_score = 0;
    best_recipe.clear();
    calories_matter = count_calories;

    for(uint32_t qty = 0; qty <= remaining_units; qty++)
    {
        remaining_units -= qty;
        recipe[0] = qty;
        next_ingr(0);
        recipe[0] = 0;
        remaining_units += qty;
    }

    return best_score;
}


int main()
{
    auto start = chrono::high_resolution_clock::now();

    // execute code
    ifstream infile("input.txt");
    if(infile.is_open())
    {
        string line;

        vector<Ingredient> ingredients;

        //parse lines
        while(getline(infile, line))
        {
            istringstream iss{line};
            string name;
            getline(iss, name, ':');
            string title;
            string value;

            iss >> title;
            getline(iss, value, ',');
            int capacity = stoi(value);

            iss >> title;
            getline(iss, value, ',');
            int durability = stoi(value);

            iss >> title;
            getline(iss, value, ',');
            int flavor = stoi(value);

            iss >> title;
            getline(iss, value, ',');
            int texture = stoi(value);

            int calories;
            iss >> title;
            iss >> calories;

            ingredients.push_back({
                name,
                capacity,
                durability,
                flavor,
                texture,
                calories});
        }

        // for(auto ing : ingredients)
        // {
        //     cout << ing.name << ":\n";
        //     cout << "  capacity: "    << ing.capacity << "\n";
        //     cout << "  durability: "  << ing.durability << "\n";
        //     cout << "  flavor: "      << ing.flavor << "\n";
        //     cout << "  texture: "     << ing.texture << "\n";
        //     cout << "  calories: "    << ing.calories << "\n";
        // }


        cout << "a) best w/o calories: " << create_recipe(ingredients, 100, false) << "\n";
        cout << "b) best w/  calories: " << create_recipe(ingredients, 100, true) << "\n";

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
