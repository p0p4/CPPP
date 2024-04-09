#include <iostream>
#include <vector>
#include <algorithm>

using std::cout;
using std::endl;

void ResetHealthPoints(std::vector<int>& health_points)
{
    static std::vector<int> default_health_points = health_points;
    health_points = default_health_points;
}

void ReduceHealth(int& health, int amount)
{
    health = (health > amount) ? health - amount : 0;
}

void PrintHealthPoints(const std::vector<int>& health_points)
{
    for (const int& health_point : health_points)
    {
        cout << health_point << " ";
    }
    cout << endl;
}

int main()
{
    std::vector<int> health_points = { 120, 90, 250, 100, 50, 200 };

    cout << "Using outside function:\n";
    ResetHealthPoints(health_points);
    PrintHealthPoints(health_points);

    std::for_each(health_points.begin(), health_points.end(), [](int& health)
        {
            ReduceHealth(health, 100);
        });

    PrintHealthPoints(health_points);
    cout << endl;

    cout << "Using function object:\n";
    ResetHealthPoints(health_points);
    PrintHealthPoints(health_points);

    struct
    {
        void operator()(int& health, int amount) const
        {
            health = (health > amount) ? health - amount : 0;
        }
    } reduce_health;

    std::for_each(health_points.begin(), health_points.end(), [&reduce_health](int& health)
        {
            reduce_health(health, 100);
        });

    PrintHealthPoints(health_points);
    cout << endl;

    cout << "Using lambda:\n";
    ResetHealthPoints(health_points);
    PrintHealthPoints(health_points);

    std::for_each(health_points.begin(), health_points.end(), [](int& health)
        {
            health = (health > 100) ? health - 100 : 0;
        });

    PrintHealthPoints(health_points);
    cout << endl;

    cout << "Using named lambda\n";
    ResetHealthPoints(health_points);
    PrintHealthPoints(health_points);

    auto reduce_health_lambda = [](int& health, int amount)
        {
            health = (health > amount) ? health - amount : 0;
        };

    std::for_each(health_points.begin(), health_points.end(), [&reduce_health_lambda](int& health)
        {
            reduce_health_lambda(health, 100);
        });

    PrintHealthPoints(health_points);
    cout << endl;

    cout << "Sorting health points\n";
    std::sort(health_points.begin(), health_points.end(), [](int a, int b)
        {
            return a > b;
        });
    // note to self: std::greater<int>()); also exists

    PrintHealthPoints(health_points);

    return 0;
}
