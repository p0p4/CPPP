#include <iostream>

using std::cout;
using std::endl;

std::mutex mtx;

std::string Timestamp();

template <typename T>
class LogPointer
{
private:
    T* _i_;
    int* _ref_count_;

public:
    LogPointer(T* i) : _i_(i)
    {
        _ref_count_ = new int(1);

        cout << Timestamp() << "| " << _i_ << " owner created" << endl;
        cout << Timestamp() << "| " << _i_ << " " << *_ref_count_ << " owner(s)" << endl;
        cout << endl;
    }

    LogPointer(const LogPointer& other)
    {
        std::lock_guard<std::mutex> lock(mtx);

        _i_ = other._i_;
        _ref_count_ = other._ref_count_;
        ++(*_ref_count_);

        cout << Timestamp() << "| " << _i_ << " owner created" << endl;
        cout << Timestamp() << "| " << _i_ << " " << *_ref_count_ << " owner(s)" << endl;
        cout << endl;
    }

    ~LogPointer()
    {
        std::lock_guard<std::mutex> lock(mtx);

        if (--(*_ref_count_) == 0)
        {
            delete _i_;
            delete _ref_count_;
        }
        cout << Timestamp() << "| " << _i_ << " owner destroyed" << endl;
        cout << Timestamp() << "| " << _i_ << " " << *_ref_count_ << " owner(s)" << endl;
        cout << endl;
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

                cout << Timestamp() << "| " << _i_ << " owner unassigned" << endl;
                cout << Timestamp() << "| " << _i_ << " " << (*_ref_count_) << " owner(s)" << endl;
            }
            cout << Timestamp() << "| " << _i_ << " = " << other._i_ << endl;

            _i_ = other._i_;
            _ref_count_ = other._ref_count_;
            ++(*_ref_count_);

            cout << Timestamp() << "| " << _i_ << " new owner assigned" << endl;
            cout << Timestamp() << "| " << _i_ << " " << *_ref_count_ << " owner(s)" << endl;
            cout << endl;
        }
        return *this;
    }

    T* operator->()
    {
        cout << Timestamp() << "| " << _i_ << " operator->" << endl;
        return _i_;
    }

    T& operator*()
    {
        cout << Timestamp() << "| " << _i_ << " operator*" << endl;
        return *_i_;
    }
};

class Test
{
private:
    int x;

public:
    Test(int x) : x(x)
    {
        cout << Timestamp() << "| " << this << " Test" << x << "()" << " created" << endl;
    }
    ~Test()
    {
        cout << Timestamp() << "| " << this << " ~Test" << x << "()" << " destroyed" << endl;
    }

    void someFunction()
    {
        cout << Timestamp() << "| " << this << " Test" << x << "::someFunction()" << endl;
        cout << endl;
    }
};

int main()
{
    cout << Timestamp() << "| {\n\n";

    // Create an object on the heap and pointer to it on the stack
    Test* _test1 = new Test(1);

    // Create a LogPointer to manage the given object on the heap
    LogPointer<Test> test_pointer1(_test1);

    {
        cout << Timestamp() << "|   {\n\n";

        // Create a LogPointer to manage the given object on the heap
        LogPointer<Test> test_pointer2(new Test(2));

        // Call a function of the object using the pointer and * operator
        (*test_pointer2).someFunction();

        // Assign a pointer to another pointer
        test_pointer2 = test_pointer1;

        cout << Timestamp() << "|   }\n\n";
    }

    // Copy the pointer to another pointer in its creation
    LogPointer<Test> test_pointer3 = test_pointer1;

    // Call a function of the object using the pointer and -> operator
    test_pointer3->someFunction();

    cout << Timestamp() << "| }\n\n";

    return 0;
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
