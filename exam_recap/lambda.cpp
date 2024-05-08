#include <iostream>

class A
{
private:
    int _x;

public:
    A(int x) : _x(x) {}
    void foo() const
    {
        static int i;
        i++;
        // _x++;  // because const x cannot be modified
        std::cout << "i = " << i << std::endl;
    }

    static int add(int a, int b)
    {
        return a + b;
    }

    int poo(int y)
    {
        const static int z = y;
        std::cout << "z = " << z << std::endl;
        return z;
    }
};

int main()
{
    int a = 0;
    A* o1 = new A(1);
    A o2(1);
    A* o3 = new A(1);
    o1->foo();     // i = 1
    o2.foo();      // i = 2
    (*o3).foo();   // i = 3
    o1->foo();     // i = 4
    o1->poo(1);    // z = 1
    o2.poo(2);     // z = 1
    (*o3).poo(3);  // z = 1

    // intentional memory leak test
    delete o1;
    delete o3;

    A o4(1);

    o4.foo();  // i = 1

    std::cout << A::add(1, 2) << std::endl;  // 3

    // making shared pointers
    std::shared_ptr<A> sp5 = std::make_shared<A>(1);

    std::shared_ptr<A> sp6 = sp5;
    std::weak_ptr<A> wp7 = sp5;

    // print reference count of sp5
    std::cout << sp5.use_count() << std::endl;  // 2

    return 0;
}
