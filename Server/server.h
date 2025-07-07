#ifndef SERVER_H     // If MY_HEADER_H is not defined
#define SERVER_H 

#include <boost/asio.hpp>
#include <iostream>
#include <boost/system/error_code.hpp>
#include <atomic>
#include <thread>
class Server{
public:
    Server(boost::asio::io_context& context, int port_number);
    ~Server();
    void accept_connection();
    void start_service(boost::asio::ip::tcp::socket& socket);
private:
    boost::asio::io_context& io_context;
    boost::asio::ip::tcp::acceptor acceptor;
    std::atomic<bool> keep_running {true};
    int port;
    std::thread t;
};
#endif // End of include guard