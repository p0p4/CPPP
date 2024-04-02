#include <iostream>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>

using namespace std;

mutex console_mtx;

class GameTask
{
private:
    int task_id;

public:
    GameTask(int id) : task_id(id) {}

    virtual ~GameTask() {}

    virtual void perform(int thread) = 0;

    int getTaskId() const
    {
        return task_id;
    }
};

class Task : public GameTask
{
public:
    Task(int id) : GameTask(id) {}

    void perform(int thread_id) override
    {
        {
            lock_guard<mutex> lock(console_mtx);
            cout << "Thread: " << thread_id << " | Task: " << getTaskId() << " started" << endl;
        }

        double x = 0;
        for (int i = 0; i < 500'000'000; ++i)
        {
            x += sin(i * 0.001);
        }

        {
            lock_guard<mutex> lock(console_mtx);
            cout << "Thread: " << thread_id << " | Task: " << getTaskId() << " finished" << endl;
        }
    }
};

class TaskQueue
{
private:
    mutex queue_mtx;
    condition_variable condition;
    queue<GameTask *> tasks;
    vector<thread> threads;
    bool stop;

public:
    TaskQueue(int nof_threads) : stop(false)
    {
        for (int i = 1; i <= nof_threads; ++i)
        {
            threads.emplace_back([this](int thread_id)
                                 { workerThread(thread_id); },
                                 i);
        }
    }

    ~TaskQueue()
    {
        {
            unique_lock<mutex> lock(queue_mtx);
            stop = true;
        }
        condition.notify_all();
        for (thread &thread : threads)
        {
            thread.join();
        }
    }

    void addJob(GameTask *task)
    {
        unique_lock<mutex> lock(queue_mtx);
        tasks.push(task);
        condition.notify_one();
    }

private:
    void workerThread(int thread_id)
    {
        while (true)
        {
            unique_lock<mutex> lock(queue_mtx);
            condition.wait(lock, [this]
                           { return !tasks.empty() || stop; });

            if (stop && tasks.empty())
            {
                return;
            }

            GameTask *task = tasks.front();
            tasks.pop();

            lock.unlock();

            task->perform(thread_id);

            delete task;

            lock.lock();

            if (tasks.empty() && !stop)
            {
                condition.wait(lock);
            }
        }
    }
};

int main()
{
    TaskQueue task_queue(3);

    for (int i = 0; i < 10; ++i)
    {
        GameTask *task = new Task(i);
        task_queue.addJob(task);
    }

    return 0;
}

/*
Q: Kun säie on saanut työn suoritettua, sen on syytä tarkistaa, olisiko jonossa uusi työ sen
suoritettavaksi, eikä mennä suoraan odottamaan ehtomuuttujan signaalia. Huomaatko miksi?

A: Jottei säie tuhlaa aikaa odottamalla ehtomuuttujan signaalia, jos jonossa on jo työtä suoritettavaksi.
*/
