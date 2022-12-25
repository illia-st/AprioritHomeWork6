#include "../Header/Server.h"
#include <iostream>


    // namespace {
    //     template <std::size_t N = Buffer_size>
    //     void printMsg(const Buffer<N>& buffer){
    //         for(size_t i{}; i < buffer.GetLastReceivedBytes(); ++i){
    //             std::cout << buffer.getBuffer().get()[i];
    //         }
    //         std::cout << std::endl;
    //     }
    // }


    TCP::Server::Server(): IServer() {}
    TCP::Server::Server(uint16_t port,KeepAlive if_live): IServer(port, if_live) {}
    int TCP::Server::StartServer() {
        Socket sock = socket(AF_INET, SOCK_STREAM, 0);
        std::cout << this->GetInfo().GetPort() << std::endl;
        if(sock == -1){
            // make loger
            int error = errno;
            return error;
        }
        int opt {1};
        if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR|SO_REUSEPORT, &opt, sizeof(opt))){
            int error = errno;
            return error;
        }
        if(bind(sock, reinterpret_cast<sockaddr*>(&this->GetInfo().GetData()), sizeof(this->GetInfo())) == -1 ||
            listen(sock, SOMAXCONN) == -1){
            // make loger
            int error = errno;
            return error;
        }
        fcntl(sock, F_SETFL, fcntl(sock, F_GETFL)|O_NONBLOCK);
        this->GetInfo().SetSocket(sock);// Connection Status is automatically updated
        // run connection acceptor
        acceptor = std::thread(&Server::AccessingNewConnections, this);
        // run data loader. This thread will receive msg from clients
        data_loader = std::thread(&Server::LoadData, this);
        // run task manager to answer client
        respond_manager = std::thread(&Server::RespondRequests, this);
        return 1;
    }
    int TCP::Server::StopServer() {
        std::cout << "I am stopping the server" << std::endl;
        if(this->GetInfo().GetConnectionStatus() == Connection::CLOSED){
            return 1;
        }
        this->GetInfo().SetConnectionStatus(Connection::CLOSED);
        std::cout << "The status is set" << std::endl;
        for(auto& clients: GetConnections()){
            if(clients->Disconnect() != 1){
                // make log about error
            }
        }
        GetConnections().clear();
        std::cout << "All the clients are disconnected now. Proof " << GetConnections().size() << std::endl;
        this->GetInfo().CloseSocket();
        acceptor.join();
        data_loader.join();
        std::cout << "Threads are being joined now" << std::endl;
        switch(GetAliveStatus()){
            case KeepAlive::FALSE: exit(EXIT_SUCCESS);
            case KeepAlive::TRUE: return 1;
        }
        return 1;
    }

    void TCP::Server::LoadData() {
        using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
            std::this_thread::sleep_for(200ms);
            ConnectionsList& cons {this->GetConnections()};
            std::scoped_lock lock(client_mutex);
            for(auto unit {cons.begin()}; unit != cons.end(); ){
                auto client {unit->get()};
                auto received_data {client->LoadData()};
                switch(received_data.GetFlag()){
                    case MsgFlag::EmptyMsg:{
                        std::cout << "Nothing to do" << std::endl;
                        break;
                    }
                    case MsgFlag::GreetingMsg:{
                        std::cout << "Respond with hello world" << std::endl;
                        std::scoped_lock lock(respond_mutex);
                        // check this out
                        responds.push({unit, std::move(received_data)});
                        break;
                    }
                    case MsgFlag::EchoMsg:{
                        std::cout << "Make echo" << std::endl;
                        // //// A message which is suitable for the server and can be parsed correctly by buffer interface
                        // printMsg(accepted_buf->GetInfo(), accepted_buf->GetSize());
                        break;
                    }
                    case MsgFlag::Disconnect:{
                        // disconnect
                        std::cout << "A disconnect from " << (*unit)->GetIP() << ":" << (*unit)->GetPort() << std::endl;
                        client->Disconnect();
                        unit = cons.erase(unit);
                        continue;
                    }
                    default:
                        // printMsg(received_data.GetBuffer.get(), received_dat);
                        break;
                }
                ++unit;
            }
        }
    }
    void TCP::Server::AccessingNewConnections(){
    using namespace std::chrono_literals;
        while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {
            struct sockaddr_in address;
            int addr_len {sizeof(address)};
            memset(&address, 0, sizeof(address));
            std::cout << "Waiting for a new connection" << std::endl;
            std::this_thread::sleep_for(100ms);
            Socket client_socket{accept(this->GetInfo().GetSocket(),
                                        reinterpret_cast<sockaddr *>(&address),
                                        reinterpret_cast<socklen_t*>(&addr_len))};
            std::cout << "Here is a new connection" << std::endl;
            if (this->GetInfo().GetConnectionStatus() != Connection::OPENED) {
                return;
            }
            if (client_socket <= -1) {
                int error = errno;
                continue;
            }
            std::cout << "Socket is fine" << std::endl;
            std::shared_ptr<CommunicationUnit> unit(
                    std::make_shared<CommunicationUnit>(CommunicationUnit(ntohs(address.sin_port),
                                                                   ntohl(address.sin_addr.s_addr))));
            unit->SetSocket(client_socket);
            unit->SetConnectionStatus(Connection::OPENED);
            std::cout << "New connection is here" << std::endl;
            std::scoped_lock lock(client_mutex);
            std::cout << "Here is a new connection from " << unit->GetIP() << ":" << unit->GetPort() << std::endl;
            IServer::GetConnections().push_back(std::move(unit));
        }
    }
    void TCP::Server::RespondRequests(){

    }
    // Set New port I will implement after Start and Stop
    TCP::ConnectionsList &TCP::Server::GetConnections() {
        std::scoped_lock lock(client_mutex);
        return IServer::GetConnections();
    }

    const TCP::ConnectionsList &TCP::Server::GetConnections() const {
        std::scoped_lock lock(client_mutex);
        return IServer::GetConnections();
    }
    void TCP::Server::SetNewPort(uint16_t port) {std::cout << "Later" << std::endl;}