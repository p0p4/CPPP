#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <iomanip>
#include <chrono>

using namespace std;

mutex mtx;
int sum = 0;

void AddNumbers(const vector<int>& nums, int start, int end, int index)
{
    int local_sum = 0;

    for (int j = start; j < end; ++j)
    {
        local_sum += nums[j];
    }

    lock_guard<mutex> lock(mtx);
    sum += local_sum;

    cout << "Thread: " << setw(3) << index
        << "| start: " << setw(12) << start
        << "| end: " << setw(12) << (end - 1)
        << "| iterations: " << setw(12) << (end - start)
        << "| sum: " << setw(12) << sum << endl;
}

int main()
{
    vector<int> nums;
    int thread_count = 0;
    int num_count = 0;

    cout << "Enter number count: ";
    cin >> num_count;
    cout << "Enter thread count: ";
    cin >> thread_count;

    if (thread_count <= 0 || num_count <= 0 || num_count < thread_count)
    {
        return 1;
    }

    for (int i = 0; i < num_count; ++i)
    {
        nums.emplace_back(1);
    }

    thread threads[thread_count];

    int thread_chunk = num_count / thread_count;
    int remainder = num_count % thread_count;

    auto start_time = chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i)
    {
        int start = i * thread_chunk + (i < remainder ? i : remainder);
        int end = start + thread_chunk + (i < remainder ? 1 : 0);

        threads[i] = thread(AddNumbers, ref(nums), start, end, i);
    }

    for (int i = 0; i < thread_count; ++i)
    {
        threads[i].join();
    }

    auto end_time = chrono::high_resolution_clock::now();
    chrono::duration<double> elapsed = end_time - start_time;

    cout << "sum: " << sum << endl;
    cout << "time: " << elapsed.count() << "s" << endl;

    return 0;
}
