#include <iostream>
#include "Header/Client.h"

int main(int argc, char** argv){
    // a simple gui
    if(argc < 3){
        std::cerr << "Usage: <host> <port>" << std::endl;
        return -1;
    }
    TCP::Client client(argv[1], std::stoi(argv[2]));
    if(client.Connect()){
        std::cout << "You have connected to the host" << std::endl;
        std::string line;
        while(std::getline(std::cin, line)){
            if(!client.SendData(line)){
                std::cout << "The msg wasn't received. Disconnect." << std::endl;
                client.Disconnect();
                break;
            }
            if(line == "Close"){
                client.Disconnect();
                break;
            }
            auto host_respond {client.LoadData()};
            if(host_respond.GetFlag() == TCP::MsgFlag::Disconnect){
                std::cout << "The host was remotely closed. Disconnect." << std::endl;
                client.Disconnect();
                break;
            }
            for(std::size_t i {0}; i < host_respond.GetLastReceivedBytes(); ++i){
                std::cout << host_respond.GetBuffer().get()[i];
            }
            std::cout << std::endl;
//            std::cin.clear();
//            std::cin.ignore(10'000, '\n');
            line.clear();
        }
    }else{
        std::cout << "The connecteion was not successful" << std::endl;
    }
    return 0;
}