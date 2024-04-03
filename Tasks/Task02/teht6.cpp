#include <iostream>
#include <vector>
#include <future>

using namespace std;

class GameTask
{
public:
    virtual ~GameTask() {}
    virtual void perform() = 0;
};

class Task : public GameTask
{
public:
    void perform() override
    {
        double x = 0;
        for (int i = 0; i < 50'000'000; ++i)
        {
            x += sqrt(i * 0.001);
        }
    }
};

int main()
{
    vector<GameTask *> tasks;
    int num_tasks = 0;

    cout << "Enter number of tasks: ";
    cin >> num_tasks;

    if (num_tasks <= 0)
        return 1;

    for (int i = 0; i < num_tasks; ++i)
    {
        tasks.emplace_back(new Task());
    }

    auto start_time = chrono::high_resolution_clock::now();

    vector<future<void>> futures;
    for (auto task : tasks)
    {
        futures.emplace_back(async(launch::async, [&task]()
                                   { task->perform(); }));
    }

    for (auto &future : futures)
    {
        future.wait();
    }

    auto end_time = chrono::high_resolution_clock::now();
    auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

    cout << "Execution time with async: " << duration << " milliseconds" << endl;

    for (auto task : tasks)
    {
        delete task;
    }

    return 0;
}
