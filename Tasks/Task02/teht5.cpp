#include <iostream>
#include <vector>
#include <algorithm>
#include <chrono>
#include <numeric>
#include <execution>

using namespace std;

int main()
{
    const int N = 10'000'000;
    vector<int> numbers(N);

    auto increment = [](int &n)
    { ++n; };

    for (int i = 0; i < 3; ++i)
    {
        iota(numbers.begin(), numbers.end(), 0);
        auto start_time = chrono::high_resolution_clock::now();
        switch (i)
        {
        case 0:
            cout << "seq:" << endl;
            for_each(execution::seq, numbers.begin(), numbers.end(), increment);
            break;
        case 1:
            cout << "par:" << endl;
            for_each(execution::par, numbers.begin(), numbers.end(), increment);
            break;
        case 2:
            cout << "par_unseq:" << endl;
            for_each(execution::par_unseq, numbers.begin(), numbers.end(), increment);
            break;
        }
        auto end_time = chrono::high_resolution_clock::now();
        chrono::duration<double> elapsed = end_time - start_time;
        cout << "time: " << elapsed.count() << "s\n\n";
    }

    return 0;
}

/*
Q1:
Havaitsetko näin yksinkertaisessa tehtävässä suorituskykyeroja eri suoritusmääreiden välillä?
Q2:
Mitä eri suoritusmääreet tarkoittavat?

A1:
par_unseq on nopein, par toiseksi nopein ja seq hitain.
A2:
std::execution::seq         |   suorittaa operaation peräkkäisesti ilman rinnakkaistamista.
std::execution::par         |   suoittaa operaation rinnakkain.
std::execution::par_unseq   |   suorittaa operaation rinnakkain ja sallii myös vektorisaation.
*/

// cd "/Users/tino/Library/Mobile Documents/com~apple~CloudDocs/School/Metropolia/Courses/CPPP/Tasks/Task02/" && clang++ -std=c++23 -fexperimental-library teht5.cpp -o teht5 && "/Users/tino/Library/Mobile Documents/com~apple~CloudDocs/School/Metropolia/Courses/CPPP/Tasks/Task02/"teht5
