#include <iostream>
#include <thread>
#include <mutex>

using namespace std;

class BankAccount
{
private:
    double balance;
    mutex mtx;

public:
    BankAccount(double balance) : balance(balance) {}

    void deposit(double amount)
    {
        lock_guard<mutex> lock(mtx);
        balance += amount;
        // cout << "Deposited: " << amount << endl;
    }

    void withdraw(double amount)
    {
        lock_guard<mutex> lock(mtx);
        if (balance >= amount)
        {
            balance -= amount;
            // cout << "Withdrawn: " << amount << endl;
        }
    }

    double get_balance()
    {
        return balance;
    }
};

int main()
{
    double withdraw_amount = 12.34;
    double deposit_amount = 12.3;
    int entries = 10000;

    BankAccount account(withdraw_amount * entries);

    thread deposit_thread([&account, deposit_amount, entries]()
                          {
        for (int i = 0; i < entries; ++i)
        {
            account.deposit(deposit_amount);
        } });

    thread withdraw_thread([&account, withdraw_amount, entries]()
                           {
        for (int i = 0; i < entries; ++i)
        {
            account.withdraw(withdraw_amount);
        } });

    withdraw_thread.join();
    deposit_thread.join();

    cout << "Balance: " << account.get_balance() << endl;

    return 0;
}
