#include <boost/asio.hpp>
#include <iostream>
#include <map>
#include <mutex>
#include <string>
#include <thread>

#define BOLDWHITE "\033[1m\033[37m"
#define RESET "\033[0m"

using boost::asio::ip::tcp;
using std::cout;
using std::endl;
using std::string;

std::map<string, tcp::socket*> clients;
std::mutex mtx;
int client_count = 0;

void handle_disconnect(const string&);

/**
 * @brief Broadcast types
 */
enum broadcast_type
{
    Message,
    Announcement,
};

/**
 * @brief Broadcast a message to connected clients
 *
 * Format message data and forward it to appropriate clients
 *
 * @param data The data to be broadcasted
 * @param sender The name of the sender
 * @param broadcast_type The type of broadcast
 */
void broadcast(const string& data, const string& sender, broadcast_type broadcast_type)
{
    string message;
    bool exclude;

    // Format message
    switch (broadcast_type)
    {
    case Message:
        message = sender + ": " + data;
        exclude = true;
        cout << message << endl;
        break;

    case Announcement:
        message = BOLDWHITE + data + RESET;
        exclude = false;
        cout << data << endl;
        break;
    }

    // Send message clients
    std::lock_guard<std::mutex> guard(mtx);
    for (auto& client : clients)
    {
        if (exclude && client.first == sender)
            continue;

        boost::asio::write(*client.second, boost::asio::buffer(message + "\n"));
    }
}

/**
 * @brief Handle client connections
 *
 * Read client name and messages
 *
 * @param socket The client socket
 */
void handle_client(tcp::socket* socket)
{
    string client_name;
    try
    {
        boost::asio::streambuf buffer;

        // Get client name
        while (true)
        {
            boost::asio::read_until(*socket, buffer, "\n");
            std::istream is(&buffer);
            std::getline(is, client_name);

            // handle duplicate names
            {
                std::lock_guard<std::mutex> guard(mtx);
                if (clients.find(client_name) != clients.end())
                {
                    boost::asio::write(*socket, boost::asio::buffer("invalid\n"));
                    client_name.clear();
                }
                else
                {
                    boost::asio::write(*socket, boost::asio::buffer("valid\n"));
                    break;
                }
            }
            buffer.consume(buffer.size());
        }

        // Add client to the list
        {
            std::lock_guard<std::mutex> guard(mtx);
            client_count++;
            clients[client_name] = socket;
        }

        broadcast(client_name + " joined, ONLINE: " + std::to_string(client_count), client_name, Announcement);

        // Wait for messages from the client
        while (true)
        {
            boost::asio::read_until(*socket, buffer, "\n");
            string message;
            std::istream is(&buffer);
            std::getline(is, message);

            if (message == "exit")
                break;

            else if (message != "")
            {
                broadcast(message, client_name, Message);
            }
            buffer.consume(buffer.size());
        }

        handle_disconnect(client_name);
    }
    catch (std::exception& e)
    {
        std::cerr << "thread: " << e.what() << endl;
        handle_disconnect(client_name);
    }
}

/**
 * @brief Handle client disconnects
 *
 * @param client_name The name of the client
 */
void handle_disconnect(const string& client_name)
{
    {
        std::lock_guard<std::mutex> guard(mtx);
        client_count--;

        broadcast(client_name + " left, ONLINE: " + std::to_string(client_count), client_name, Announcement);

        // delete socket
        delete clients[client_name];
        clients.erase(client_name);
    }
}

/**
 * @brief Main function
 *
 * Accept clients and create a new thread for each
 *
 * @return int
 */
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
            std::cerr << "main: " << e.what() << endl;
        }
    }

    return 0;
}
