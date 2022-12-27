#pragma once

#include <unordered_map>
#include <thread>
#include <mutex>
#include <string_view>
#include <algorithm>
#include <fcntl.h>
#include "IClient.h"

class TCP::Client : public TCP::IClient{
public:
    Client() = default;
    Client(uint32_t host, uint16_t port);
    Client(const std::string& IP, uint16_t port);
    ~Client();
    int Connect() override;
    int Disconnect() override;
    Buffer LoadData() override;
    int SendData(const std::string& msg);
};