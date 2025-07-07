#include "client.h"
#include <thread>
Client::Client( boost::asio::io_context &context)
: context_ref(context)
, sck(context)
{
}

Client::~Client()
{
    sck.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
    sck.close();
}

std::pair<int, std::string> Client::connect(const std::string &address, const std::string &port_number)
{
    boost::asio::ip::tcp::resolver resolver(context_ref);
    auto end_points = resolver.resolve(address, port_number,ec);
    if (ec.value() != 0) {
        return {ec.value(), ec.message()};
    }
    for (const auto& e_point : end_points) {
        sck.connect(e_point,ec);
        if (ec.value() != 0) {
            return {ec.value(), ec.message()};
        }
        ep = e_point;
        return {0, "Success"};
    }
    return {-1, "No end points resolved"};
}

std::pair<int, std::string> Client::emulate_some_communication(const std::string& msg)
{
    std::string server_msg = msg + "\n";
    auto res = send_request(server_msg);
    if (res.first != 0) return res;

    std::this_thread::sleep_for(std::chrono::milliseconds(200));
    return get_response();
}

std::pair<int, std::string> Client::send_request(const std::string &msg)
{
    auto asio_buffer = boost::asio::buffer(msg);
    boost::asio::write(sck, asio_buffer, ec);
    if (ec.value() != 0) {
        return {ec.value(), ec.message()};
    }
    return {0, "Request sent Succ...."};
}

std::pair<int, std::string> Client::get_response()
{
    boost::asio::streambuf buffer;
    boost::asio::read_until(sck, buffer,"\n",ec);
    if (ec.value() != 0) {
        return {ec.value(), ec.message()};
    }
    std::istream input_stream (&buffer);
    std::string response;
    std::getline(input_stream, response);
    return {0, response};
}
