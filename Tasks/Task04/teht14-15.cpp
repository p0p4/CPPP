#include <iostream>

class BigData
{
private:
    char* buffer;
    int size;

public:
    BigData(int size) : size(size)
    {
        buffer = (char*)malloc(size);
    }

    BigData(const BigData& other) : size(other.size)
    {
        buffer = (char*)malloc(size);
        memcpy(buffer, other.buffer, size);
    }

    BigData(BigData&& other)
    {
        size = other.size;
        buffer = other.buffer;
        other.buffer = nullptr;
    }

    ~BigData()
    {
        free(buffer);
    }

    BigData& operator=(const BigData& other)
    {
        if (this != &other)
        {
            free(buffer);
            size = other.size;
            buffer = (char*)malloc(size);
            memcpy(buffer, other.buffer, size);
        }
        return *this;
    }

    BigData& operator=(BigData&& other)
    {
        if (this != &other)
        {
            free(buffer);
            size = other.size;
            buffer = other.buffer;
            other.buffer = nullptr;
        }
        return *this;
    }
};

int main()
{
    BigData a(1024);
    BigData b(1024);
    a = a;
    a = b;
    BigData c(a);

    BigData d(BigData(1024));
    BigData e(1024);
    e = BigData(1024);
    BigData f(std::move(d));

    return 0;
}
