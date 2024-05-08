#include <boost/asio.hpp>
#include <iostream>
#include <thread>

using boost::asio::ip::tcp;

void receive_messages(tcp::socket& socket)
{
    try
    {
        while (true)
        {
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, "\n");
            std::string message;
            std::istream is(&buffer);
            std::getline(is, message);
            std::cout << message << std::endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in receive thread: " << e.what() << std::endl;
    }
}

int main()
{
    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        auto endpoints = resolver.resolve("localhost", "8080");
        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        std::cout << "Enter your name: ";
        std::string name;
        std::getline(std::cin, name);
        name += "\n";

        boost::asio::write(socket, boost::asio::buffer(name));

        std::thread receive_thread([&socket]()
                                   { receive_messages(socket); });

        while (true)
        {
            std::string input;
            std::getline(std::cin, input);

            if (input == "exit")
            {
                boost::asio::write(socket, boost::asio::buffer("exit\n"));
            }
            else
            {
                boost::asio::write(socket, boost::asio::buffer(input + "\n"));
            }
        }

        receive_thread.join();
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in main: " << e.what() << std::endl;
    }

    return 0;
}
