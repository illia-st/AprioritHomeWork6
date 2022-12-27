#include "../Header/Client.h"

TCP::Client::Client(uint32_t host, uint16_t port): IClient(host, port) {}

TCP::Client::Client(const std::string& IP, uint16_t port): IClient(IP, port) {}

int TCP::Client::Connect(){
    Socket sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
    if(sock == -1) {return static_cast<int>(errno);}
    auto respond {connect(sock, reinterpret_cast<struct sockaddr*>(&GetHostInfo().GetData()), sizeof(GetHostInfo().GetData()))};
    if(respond == -1){
        GetHostInfo().CloseSocket();
        return static_cast<int>(errno);
    }
    int flags = fcntl(sock, F_GETFL);
     if (fcntl(sock, F_SETFL, flags | O_NONBLOCK) == -1) {
        GetHostInfo().CloseSocket();
        return static_cast<int>(errno);
    }
    GetHostInfo().SetSocket(sock);

    return 1;
}

int TCP::Client::Disconnect(){
    return GetHostInfo().Disconnect();
}
TCP::Buffer TCP::Client::LoadData(){
    auto received_info {GetHostInfo().LoadData()};

    if(received_info.GetFlag() == MsgFlag::Disconnect){
        GetHostInfo().Disconnect();
    }

    return std::move(received_info);
}
int TCP::Client::SendData(const std::string& msg){
    Buffer buffer;
    std::shared_ptr<char[]> ptr {std::make_shared<char[]>(msg.length() + 1)};
    if(msg == "Hello"){
        ptr.get()[0] = static_cast<uint8_t>(MsgFlag::GreetingMsg);
    }else{
        ptr.get()[0] = static_cast<uint8_t>(MsgFlag::EchoMsg);
    }
    std::size_t offset {0};
    std::fill(ptr.get() + 1, ptr.get() + msg.length() + 1, msg[offset++]);
    auto res { send(GetHostInfo().GetSocket(), static_cast<void*>(ptr.get()),
                                msg.length() + 1, 0)};
    if(res == -1) {
        int error { static_cast<int>(errno) };
        return 0;
    }

    return static_cast<int>(res);
}