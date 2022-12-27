#include "CommunicationUnit.h"
#include <iostream>

namespace TCP {

    CommunicationUnit::CommunicationUnit() : m_sock(-1), con(Connection::CLOSED) {
        memset(&addr, 0, sizeof(struct sockaddr_in));
    }
    CommunicationUnit::~CommunicationUnit(){
        Clear();
    }

    CommunicationUnit::CommunicationUnit(uint16_t port, uint32_t host) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (host == INADDR_ANY) {
            addr.sin_addr.s_addr = INADDR_ANY;
            return;
        }
        addr.sin_addr.s_addr = htonl(host);
    }
    CommunicationUnit::CommunicationUnit(uint16_t port, const std::string& host) {
        addr.sin_family = AF_INET;
        addr.sin_port = htons(port);
        if (host == "127.0.0.1") {
            addr.sin_addr.s_addr = INADDR_ANY;
            return;
        }
        inet_pton(AF_INET, host.data(), &addr.sin_addr);
    }

    void CommunicationUnit::CloseSocket() {
//        shutdown(m_sock, SHUT_RDWR);
        close(m_sock);
        m_sock = -1;
        con = Connection::CLOSED;
    }

    int CommunicationUnit::Disconnect() {
        std::cout << "Disconnect" << std::endl;
        Clear();
        return 1;
    }

    int CommunicationUnit::SendMsg(const char *buf, size_t N) {
        int error_code = send(this->GetSocket(), reinterpret_cast<const void*>(buf), N, 0);
        if(error_code == -1){
            int error = errno;
            // make log about errno
            return error;
        }
        return 1;
    }
    void CommunicationUnit::SetSocket(Socket sock) {
        if (sock >= 0) {
            m_sock = sock;
            con = Connection::OPENED;
        }
    }

    void CommunicationUnit::Clear() {
        this->CloseSocket();
//        memset(&addr, 0, sizeof(struct sockaddr_in));
    }

    uint16_t CommunicationUnit::GetPort() const {
        return ntohs(addr.sin_port);
    }

    uint32_t CommunicationUnit::GetHost() const {
        return ntohl(addr.sin_addr.s_addr);
    }

    std::string CommunicationUnit::GetIP() const {
        return std::move(std::string((inet_ntoa(addr.sin_addr))));
    }

    void CommunicationUnit::SetConnectionStatus(Connection connection) {
        con = connection;
    }

    Connection CommunicationUnit::GetConnectionStatus() const {
        return con;
    }

    Socket &CommunicationUnit::GetSocket() { return m_sock; }

    struct sockaddr_in& CommunicationUnit::GetData() {return addr;}
}