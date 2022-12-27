#pragma once
#include "Core.h"
#include "Buffer.h"
#include <unistd.h>
#include <cstring>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

namespace TCP {

    class CommunicationUnit {
    public:
        CommunicationUnit();

        virtual ~CommunicationUnit();

        explicit CommunicationUnit(uint16_t port, uint32_t host = INADDR_ANY);

        void SetSocket(Socket sock);

        void CloseSocket();

        void SetConnectionStatus(Connection connection);

        Connection GetConnectionStatus() const;

        Socket &GetSocket();

        virtual int Disconnect();

        virtual Buffer LoadData() { return Buffer();}

        int SendMsg(const char *buf, size_t N);

        [[nodiscard]] uint16_t GetPort() const;

        [[nodiscard]] uint32_t GetHost() const;

        [[nodiscard]] std::string GetIP() const;
        
        struct sockaddr_in& GetData();
    protected:
        void Clear();
        Socket m_sock;
        struct sockaddr_in addr;
    private:
        Connection con{};
    };
    
}