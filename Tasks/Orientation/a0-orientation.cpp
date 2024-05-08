#include <cstring>
#include <forward_list>
#include <iostream>
#include <memory>
#include <set>

using namespace std;

struct string_elem_t
{
    const char* str_;
    string_elem_t* next_;
};

// // C version
// int num_hamlet(string_elem_t *books)
// {
//     const char *hamlet = "Hamlet";
//     int n = 0;
//     string_elem_t *b;

//     for (b = books; b != 0; b = b->next_)
//     {
//         if (strcmp(b->str_, hamlet) == 0)
//         {
//             ++n;
//         }
//     }

//     return n;
// }

// class Bagel
// {
// public:
//     Bagel(set<string> ts) : toppings_(move(ts)) {}

// private:
//     set<string> toppings_;
// }

// C++ version
// int num_hamlet(const forward_list<string> &books)
// {
//     return count(books.begin(), books.end(), "Hamlet");
// }

void swap(int*& a, int*& b)
{
    int temp = *a;
    *a = *b;
    *b = temp;
}

int main()
{
    int books_amount = 5;
    static int s_books_amount = 5;
    const int k_books_amount = 5;
    int* p_books_amount = &s_books_amount;

    int* a = new int(5);
    int* b = new int(10);
    swap(a, b);
    std::cout << "a = " << *a << ", b = " << *b << std::endl;

    int n = 0;

    int* p_n = &n;

    // const char *hamlet = "Hamlet";

    // cout << "hamlet = " << hamlet << endl;
    // cout << "&hamlet = " << &hamlet << endl;
    // cout << "(void *)hamlet = " << (void *)hamlet << endl;

    cout << "n = " << n << endl;
    cout << "&n = " << &n << endl;
    cout << "p_n = " << p_n << endl;
    cout << "*p_n = " << *p_n << endl;
    cout << "&p_n = " << &p_n << endl;

    // forward_list<string> books = {"Hamlet", "Macbeth", "Hamlet", "Romeo and Juliet", "Hamlet"};

    // n = num_hamlet(books);

    // cout << "n = " << n << endl;

    // std::shared_ptr<int> ptr1 = std::make_shared<int>(42);
    // std::shared_ptr<int> ptr2 = ptr1; // Shared ownership

    // weak reference
    // std::weak_ptr<int> weak1 = ptr1;

    // cout << "ptr1 = " << ptr1 << endl;
    // cout << "ptr2 = " << ptr2 << endl;
    // cout << "weak1 = " << weak1.lock() << endl;

    // cout << ptr1.use_count() << endl;  // Outputs: 2
    // cout << ptr2.use_count() << endl;  // Outputs: 2
    // cout << weak1.use_count() << endl; // Outputs: 2

    return 0;
}
