#pragma once
#include <memory>
#include <list>

namespace TCP{
    const std::size_t Buffer_size = 1024;
    using Socket = int;

    enum class KeepAlive {
        TRUE,
        FALSE
    };
    enum class Connection{
        CLOSED,
        OPENED
    };

    class CommunicationUnit;
    class Host;
    class Session;
    using ServerInfo = CommunicationUnit;
    using ConnectionsList = std::list<std::shared_ptr<Session>>;

    class IServer;
    class IClient;
    class Client;
    class Server;


}