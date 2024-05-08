#include <boost/asio.hpp>
#include <iostream>
#include <map>
#include <string>
#include <thread>

using boost::asio::ip::tcp;

std::map<std::string, tcp::socket*> clients;
std::mutex mtx;

void broadcast(const std::string& message, std::string sender)
{
    std::lock_guard<std::mutex> guard(mtx);
    for (auto& pair : clients)
    {
        if (pair.first != sender)
        {
            boost::asio::write(*(pair.second), boost::asio::buffer(message + "\n"));
        }
    }
    std::cout << message << std::endl;
}

void handle_client(tcp::socket* socket)
{
    try
    {
        boost::asio::streambuf buffer;
        std::string client_name;

        while (client_name.empty())
        {
            boost::asio::read_until(*socket, buffer, "\n");
            std::istream is(&buffer);
            std::getline(is, client_name);
        }

        {
            std::lock_guard<std::mutex> guard(mtx);
            clients[client_name] = socket;
        }

        broadcast(client_name + " online", client_name);

        while (true)
        {
            boost::asio::read_until(*socket, buffer, "\n");
            std::string message;
            std::istream is(&buffer);
            std::getline(is, message);

            if (message == "exit")
            {
                {
                    std::lock_guard<std::mutex> guard(mtx);
                    broadcast(client_name + " offline", client_name);
                }
                break;
            }
            else if (message != "")
            {
                broadcast(client_name + ": " + message, client_name);
            }
            buffer.consume(buffer.size());
        }

        clients.erase(client_name);
        delete socket;
    }
    catch (std::exception& e)
    {
        std::cerr << "Exception in thread: " << e.what() << std::endl;
    }
}

int main()
{
    boost::asio::io_context io_context;
    tcp::acceptor acceptor(io_context, tcp::endpoint(tcp::v4(), 8080));

    while (true)
    {
        try
        {
            tcp::socket* socket = new tcp::socket(io_context);
            acceptor.accept(*socket);
            std::thread([socket]
                        { handle_client(socket); })
                .detach();
        }
        catch (std::exception& e)
        {
            std::cerr << "Exception in main: " << e.what() << std::endl;
        }
    }

    return 0;
}
