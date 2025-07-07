#include "server.h"
#include "../Client/client.h"
Server::Server(boost::asio::io_context& context, int port_number) : io_context(context)
, acceptor(context)
, port(port_number)
{
   acceptor.open(boost::asio::ip::tcp::v4());
   acceptor.bind(boost::asio::ip::tcp::endpoint(boost::asio::ip::address_v4::any(), port_number));
    acceptor.listen();
    t = std::thread([&](){
        accept_connection();
    });
}

Server::~Server()
{
    keep_running = false;
    /**
     * Below small code is a quick fix to shut down server in this synchronus calls.
     */
    Client c(io_context);
    c.connect("127.0.0.1",std::to_string(port));
    c.emulate_some_communication("exit\n");

    if (acceptor.is_open()) acceptor.close();
    if (t.joinable()) t.join();
}

void Server::accept_connection()
{
    try {
        while (keep_running) {
            boost::asio::ip::tcp::socket sck (io_context);
            acceptor.accept(sck);
            start_service(sck);
        }
    }
    catch (const boost::system::system_error& e) {
        std::cout << "Server stopped accepting" << std::endl;
    }
}

void Server::start_service(boost::asio::ip::tcp::socket& socket)
{
    try {
        bool should_break = false;
        while (!should_break) {
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer,"\n");
            std::string response;
            std::istream input (&buffer);
            std::getline(input, response);
            if (response == "exit") {
                response = "Exiting you are not fun....\n";
                should_break = true;
            }
            else response += "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
            auto network_buffer = boost::asio::buffer(response);
            boost::asio::write(socket, network_buffer);
        }
        socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket.close();
    }
    catch (const boost::system::system_error& e) {
        std::cerr << "Client disconnected: " << e.what() << "\n";
    }
    
}
