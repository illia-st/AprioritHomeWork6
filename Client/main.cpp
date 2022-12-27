#include <iostream>
#include "Header/Client.h"

int main(int argc, char** argv){
    // a simple gui
    if(argc < 3){
        std::cerr << "Usage: <host> <port>" << std::endl;
        return -1;
    }
    TCP::Client client(argv[1], std::atoi(argv[2]));
    if(client.Connect()){
        std::cout << "You have connected to the host" << std::endl;
        std::string line;
        while(std::getline(std::cin, line)){
            if(!client.SendData(line)){
                std::cout << "The msg wasn't received. Disconnect." << std::endl;
                client.Disconnect();
                break;
            }
            auto host_respond {client.LoadData()};
            if(host_respond.GetFlag() == TCP::MsgFlag::Disconnect){
                std::cout << "The host was remotely closed. Disconnect." << std::endl;
                client.Disconnect();
                break;
            }
        }
    }else{
        std::cout << "The connecteion was not successful" << std::endl;
    }
    return 0;
}