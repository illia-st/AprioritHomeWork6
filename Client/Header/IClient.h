#pragma once
#include "../Common/Core.h"
#include "Host.h"


class TCP::IClient{
public:
    IClient() = default;
    virtual ~IClient() = default;
    IClient(const std::string& IP, uint16_t port);
    IClient(uint32_t host, uint16_t port);
    virtual int Connect() = 0;
    virtual int Disconnect() = 0;
    virtual Buffer LoadData() = 0;
protected:
    ServerInfo& GetHostInfo();
    const ServerInfo& GetHostInfo() const;
private:
    Host m_host;
};