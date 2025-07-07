
#include "Client/client.h"
#include "Server/server.h"
#include <string>
int main(int argc, char* argv[]) {
    if (argc < 2){
        std::cout << "Please enter valid pornt number" << std::endl;
        return 0;
    } 
    boost::asio::io_context io;
    int port = std::atoi(argv[1]);
    Server server(io, port);
    Client c(io);
    auto result = c.connect("127.0.0.1",std::to_string(port));
    if (result.first != 0) {
        std::cout << "Can not connect client something went wrong" << std::endl;
        return 0;
    }
    std::string command;
    while (true) {
        std::cout << "> ";
        std::getline(std::cin, command);
        auto res = c.emulate_some_communication(command);
        if (res.first == 0) std::cout << "Server: " << res.second << std::endl;
        else {
            std::cout << "Response went wrong: " << res.second << std::endl;
            break;
        }
        if (command == "exit") break;
        
    }
    return 0;
}
