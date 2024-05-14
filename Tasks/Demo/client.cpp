#include <boost/asio.hpp>
#include <iostream>
#include <thread>

// #include "boost/asio.hpp"

#define BOLDWHITE "\033[1m\033[37m"
#define RESET "\033[0m"

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using std::string;

/**
 * @brief Read a response from the server
 *
 * @param socket The socket to read from
 * @return string The response from the server
 */
string read_response(tcp::socket& socket)
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(socket, buffer, "\n");
    string response;
    std::istream is(&buffer);
    std::getline(is, response);
    return response;
}

/**
 * @brief Wait for messages from the server
 *
 * @param socket The socket to receive messages from
 */
void receive_messages(tcp::socket& socket)
{
    try
    {
        // Wait for messages from the server
        while (true)
        {
            string response = read_response(socket);

            cout << response << endl;
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "thread: " << e.what() << endl;
    }
}

/**
 * @brief Main function
 *
 * Connect to the server and handle user input
 *
 * @return int
 */
int main()
{
    string host = "127.0.0.1";
    string port = std::to_string(8080);

    try
    {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);

        auto endpoints = resolver.resolve(host, port);

        tcp::socket socket(io_context);
        boost::asio::connect(socket, endpoints);

        while (true)
        {
            // Send the client's name to the server
            cout << BOLDWHITE;
            cout << "Enter your name: ";
            cout << RESET;

            string name;
            std::getline(std::cin, name);
            name += "\n";

            boost::asio::write(socket, boost::asio::buffer(name));

            // Wait for a response from the server to check if the name is valid
            string response = read_response(socket);

            if (response == "valid")
            {
                break;
            }
            else
            {
                cout << BOLDWHITE;
                cout << "Ivalid name, please try again.";
                cout << RESET << endl;
            }
        }

        cout << BOLDWHITE;
        cout << "Type 'exit' to leave the chat";
        cout << RESET << endl;

        // Start a thread to receive messages from the server
        std::thread receive_thread([&socket]()
                                   { receive_messages(socket); });

        // Wait for user input
        while (true)
        {
            string input;
            std::getline(std::cin, input);

            if (input == "exit")
            {
                boost::asio::write(socket, boost::asio::buffer("exit\n"));
                break;
            }
            else
            {
                boost::asio::write(socket, boost::asio::buffer(input + "\n"));
            }
        }

        receive_thread.join();
        socket.close();
    }
    catch (std::exception& e)
    {
        std::cerr << "main: " << e.what() << endl;
    }

    return 0;
}
