#ifndef CLIENT_H     // If MY_HEADER_H is not defined
#define CLIENT_H 

#include <boost/asio.hpp>
#include <iostream>
#include <boost/system/error_code.hpp>
class Client {
public:
    Client( boost::asio::io_context& context);
    ~Client();
    std::pair<int, std::string> connect(const std::string& address, const std::string& port_number);
    std::pair<int, std::string> emulate_some_communication(const std::string& msg);
    std::pair<int, std::string> send_request(const std::string& msg);
    std::pair<int, std::string> get_response();
private:
    boost::asio::io_context& context_ref;
    boost::asio::ip::tcp::socket sck;
    boost::asio::ip::tcp::endpoint ep;
    boost::system::error_code ec;
};
#endif // End of include guard