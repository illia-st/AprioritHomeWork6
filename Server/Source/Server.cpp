#include "../Header/Server.h"
#include <iostream>



TCP::Server::Server(): IServer() {}
TCP::Server::Server(uint16_t port,KeepAlive if_live): IServer(port, if_live) {}
int TCP::Server::StartServer() {
    Socket sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        // make loger
        int error = errno;
        return error;
    }
    int opt {1};
    if(setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt)) == -1){
        int error = errno;
        return error;
    }
    if(bind(sock, reinterpret_cast<struct sockaddr*>(&this->GetInfo().GetData()), sizeof(this->GetInfo())) == -1 ||
        listen(sock, SOMAXCONN) == -1){
        // make loger
        int error = errno;
        return error;
    }

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
    if(this->GetInfo().GetConnectionStatus() == Connection::CLOSED){
        return 1;
    }
    this->GetInfo().SetConnectionStatus(Connection::CLOSED);

    for(auto& clients: GetConnections()){
        if(clients->Disconnect() != 1){
            // make log about error
        }
    }
    GetConnections().clear();
    this->GetInfo().CloseSocket();
    acceptor.join();
    data_loader.join();
    respond_manager.join();
    switch(GetAliveStatus()){
        case KeepAlive::FALSE: exit(EXIT_SUCCESS);
        case KeepAlive::TRUE: return 1;
    }
    return 1;
}

void TCP::Server::LoadData() {
    using namespace std::chrono_literals;
    while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
        std::this_thread::sleep_for(300ms);
        ConnectionsList& cons {this->GetConnections()};
        std::scoped_lock lock(client_mutex);

        for(auto unit {cons.begin()}; unit != cons.end(); ){

            auto client {unit->get()};
            auto received_data {client->LoadData()};
            switch(received_data.GetFlag()){
                case MsgFlag::EmptyMsg:{
                    ++unit;
                    break;
                }
                case MsgFlag::GreetingMsg:{
                    std::cout << "Respond with hello world" << std::endl;
                    // move down to EchoMsg just to add a new task
                    goto FROM_GREETING;
                }
                case MsgFlag::EchoMsg:{
                    std::cout << "Make echo" << std::endl;
                    FROM_GREETING:
                    // A message which is suitable for the server and can be parsed correctly by buffer interface
                    std::scoped_lock lock(respond_mutex);
                    for(std::size_t i {1}; i < received_data.GetLastReceivedBytes(); ++i){
                        std::cout << received_data.GetBuffer().get()[i];
                    }
                    std::cout << std::endl;
                    // check this out
                    std::pair<std::shared_ptr<Session>, Buffer> pr = {*unit, std::move(received_data)};
                    responds.push(std::move(pr));
                    respond_cv.notify_one();
                    ++unit;
                    break;
                }
                case MsgFlag::Disconnect:{
                    // disconnect
                    std::cout << "A disconnect from " << (*unit)->GetIP() << ":" << (*unit)->GetPort() << std::endl;
                    client->Disconnect();
                    std::cout << client->GetSocket() << std::endl;
                    unit = cons.erase(unit);
                    std::cout << "The client was removed" << std::endl;
                    if(unit == cons.end()){
                        std::cout << "No connection" << std::endl;
                    }
                    break;
                }
                default:
                    ++unit;
                    break;
            }
        }
    }
}
void TCP::Server::AccessingNewConnections(){
using namespace std::chrono_literals;
        
    while(this->GetInfo().GetConnectionStatus() == Connection::OPENED) {

        struct sockaddr_in address;
        socklen_t addrLen {sizeof(struct sockaddr_in)};
        memset(&address, 0, sizeof(address));

        Socket client_socket = -1;

        client_socket = accept(this->GetInfo().GetSocket(),
                               reinterpret_cast<struct sockaddr *>(&address),
                               &addrLen);
        if(client_socket >= 0 && this->GetInfo().GetConnectionStatus() == Connection::OPENED){
            std::shared_ptr<Session> unit(
                    std::make_shared<Session>(Session(ntohs(address.sin_port),
                                                      ntohl(address.sin_addr.s_addr))));
            unit->SetSocket(client_socket);
            unit->SetConnectionStatus(Connection::OPENED);
            std::scoped_lock lock(client_mutex);
            std::cout << "Here is a new connection from " << unit->GetIP() << ":" << unit->GetPort() << std::endl;
            IServer::GetConnections().push_back(std::move(unit));
        }
    }
}
void TCP::Server::RespondRequests(){
    while(this->GetInfo().GetConnectionStatus() == Connection::OPENED){
        std::pair<std::shared_ptr<CommunicationUnit>, Buffer> task;
        std::unique_lock<std::mutex> lock(respond_mutex, std::defer_lock);

        lock.lock();
        if(responds.empty()){
            respond_cv.wait(lock);
            if(this->GetInfo().GetConnectionStatus() == Connection::CLOSED){
                lock.unlock();
                return;
            }
        }
        task = std::move(responds.front());  
        responds.pop();
        lock.unlock();
        switch(task.second.GetFlag()){
            case MsgFlag::EchoMsg:{
                std::size_t sent_bytes = send(task.first->GetSocket(), static_cast<void*>(task.second.GetBuffer().get() + 1),
                                task.second.GetLastReceivedBytes() - 1, 0);
                if(sent_bytes == -1){
                    int error = errno;
                    // make log
                }
                break;  
            }
            case MsgFlag::GreetingMsg:{
                std::string greeting {"world"};
                std::size_t sent_bytes = send(task.first->GetSocket(), greeting.data(),
                                greeting.length(), 0);
                if(sent_bytes == -1){
                    int error = errno;
                    // make log
                }
                break;  
            }
        }
    }
}
TCP::ConnectionsList &TCP::Server::GetConnections() {
    std::scoped_lock lock(client_mutex);
    return IServer::GetConnections();
}

const TCP::ConnectionsList &TCP::Server::GetConnections() const {
    std::scoped_lock lock(client_mutex);
    return IServer::GetConnections();
}
// Not finished with this one
void TCP::Server::SetNewPort(uint16_t port) {std::cout << "Later" << std::endl;}