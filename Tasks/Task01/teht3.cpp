#include <iostream>
#include <vector>
#include <thread>

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
        for (int i = 0; i < 5000000; ++i)
        {
            x += sin(i * 0.001);
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

    for (int num_threads = 1; num_threads <= thread::hardware_concurrency(); ++num_threads)
    {
        auto start_time = chrono::high_resolution_clock::now();

        vector<thread> threads;
        for (int i = 0; i < num_threads; ++i)
        {
            threads.emplace_back([&tasks, i, num_threads]()
                                 {
                for (int j = i; j < tasks.size(); j += num_threads)
                {
                    tasks[j]->perform();
                } });
        }

        for (auto &thread : threads)
        {
            thread.join();
        }

        auto end_time = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::milliseconds>(end_time - start_time).count();

        cout << "Execution time with " << num_threads << " thread(s): " << duration << " milliseconds" << endl;

        threads.clear();
    }

    for (auto task : tasks)
    {
        delete task;
    }

    return 0;
}
