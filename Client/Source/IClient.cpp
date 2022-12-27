#include "../Header/IClient.h"

TCP::IClient::IClient(uint32_t host, uint16_t port): m_host(port, host) {}

TCP::IClient::IClient(const std::string& IP, uint16_t port): m_host(port, IP) {}

TCP::ServerInfo &TCP::IClient::GetHostInfo() {return m_host;}

const TCP::ServerInfo &TCP::IClient::GetHostInfo() const {return m_host;}