#include <iostream>
#include "Header/Server.h"

int main(int argc, char** argv){
    // a simple gui. User can also use server with KEEP_ALIVE status
    // if(argc < 2) {
    //     std::cerr << "Using: <port>" << std::endl;
    //     return 1;
    // }
    // TCP::Server server(std::stoi(argv[1]));
     
    TCP::Server server(12345);
    if(server.StartServer() == 1){
        std::string line {};
        std::cout << "Type STOP to close server" << std::endl;
        while(std::getline(std::cin, line, '\n')){
            // std::getline(std::cin, line);
            if(line == "STOP"){
                server.StopServer();
                break;
            }
            std::cin.clear();
            std::cin.ignore(10'000,'\n');
            line.clear();
        }
    }

    return 0;
}