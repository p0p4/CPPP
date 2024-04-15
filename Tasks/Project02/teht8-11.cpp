#include <iostream>
#include <fstream>
#include <sstream>
#include <mutex>

using std::cout;
using std::endl;

std::mutex mtx;

std::string Timestamp();
void WriteToLog(const std::string& message);

template <typename T>
class LogPointer
{
private:
    T* _i_;
    int* _ref_count_;
    std::stringstream pointer_string;

public:
    LogPointer(T* i) : _i_(i)
    {
        _ref_count_ = new int(1);

        pointer_string << _i_;
        WriteToLog(pointer_string.str() + " owner created");
        WriteToLog(pointer_string.str() + " " + std::to_string(*_ref_count_) + " owner(s)");
    }

    LogPointer(const LogPointer& other)
    {
        std::lock_guard<std::mutex> lock(mtx);

        _i_ = other._i_;
        _ref_count_ = other._ref_count_;
        ++(*_ref_count_);

        pointer_string << _i_;
        WriteToLog(pointer_string.str() + " owner created");
        WriteToLog(pointer_string.str() + " " + std::to_string(*_ref_count_) + " owner(s)");

    }

    ~LogPointer()
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (--(*_ref_count_) == 0)
        {
            delete _i_;
            delete _ref_count_;
        }
        WriteToLog(pointer_string.str() + " owner destroyed");
        WriteToLog(pointer_string.str() + " " + std::to_string(*_ref_count_) + " owner(s)");
    }

    LogPointer& operator=(const LogPointer& other)
    {
        if (this != &other)
        {
            std::lock_guard<std::mutex> lock(mtx);

            if (--(*_ref_count_) == 0)
            {
                delete _i_;
                delete _ref_count_;

                WriteToLog(pointer_string.str() + " owner unassigned");
                WriteToLog(pointer_string.str() + " " + std::to_string(*_ref_count_) + " owner(s)");
            }
            WriteToLog(pointer_string.str() + " = " + other.pointer_string.str());

            pointer_string.str("");
            pointer_string << other._i_;

            _i_ = other._i_;
            _ref_count_ = other._ref_count_;
            ++(*_ref_count_);

            WriteToLog(pointer_string.str() + " new owner assigned");
            WriteToLog(pointer_string.str() + " " + std::to_string(*_ref_count_) + " owner(s)");
        }
        return *this;
    }

    T* operator->()
    {
        WriteToLog(pointer_string.str() + " operator->");
        return _i_;
    }

    T& operator*()
    {
        WriteToLog(pointer_string.str() + " operator*");
        return *_i_;
    }
};

class Test
{
private:
    int x;
    std::stringstream pointer_string;

public:
    Test(int x) : x(x)
    {
        pointer_string << this;
        WriteToLog(pointer_string.str() + " Test" + std::to_string(x) + "() created");
    }
    ~Test()
    {
        WriteToLog(pointer_string.str() + " ~Test" + std::to_string(x) + "() destroyed");
    }

    void someFunction()
    {
        WriteToLog(pointer_string.str() + " Test" + std::to_string(x) + "::someFunction()");
    }
};

int main()
{
    WriteToLog("{");

    // Create an object on the heap and pointer to it on the stack
    Test* _test1 = new Test(1);

    // Create a LogPointer to manage the given object on the heap
    LogPointer<Test> test_pointer1(_test1);

    {
        WriteToLog("  {");

        // Create a LogPointer to manage the given object on the heap
        LogPointer<Test> test_pointer2(new Test(2));

        // Call a function of the object using the pointer and * operator
        (*test_pointer2).someFunction();

        // Assign a pointer to another pointer
        test_pointer2 = test_pointer1;

        WriteToLog("  }");
    }

    // Copy the pointer to another pointer in its creation
    LogPointer<Test> test_pointer3 = test_pointer1;

    // Call a function of the object using the pointer and -> operator
    test_pointer3->someFunction();

    WriteToLog("}");

    return 0;
}

void WriteToLog(const std::string& message)
{
    static std::ofstream log_file;

    log_file.open("log.txt", std::ios_base::app);
    log_file << Timestamp() << "| " << message << std::endl;
    log_file.close();

    cout << Timestamp() << "| " << message << " " << std::endl;
}

std::string Timestamp()
{
    auto now = std::chrono::system_clock::now();
    auto now_ms = std::chrono::time_point_cast<std::chrono::microseconds>(now);
    auto epoch = now_ms.time_since_epoch();
    auto value = std::chrono::duration_cast<std::chrono::microseconds>(epoch);
    auto ms = value.count() % 1000000;

    time_t tt = std::chrono::system_clock::to_time_t(now);
    tm local_tm = *localtime(&tt);

    char buffer[32];

    strftime(buffer, 32, "%H:%M:%S", &local_tm);
    snprintf(buffer, 32, "%s.%06lld", buffer, ms);

    return std::string(buffer);
}
