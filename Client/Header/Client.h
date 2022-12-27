#pragma once

#include <unordered_map>
#include <thread>
#include <mutex>
#include <string_view>
#include <fcntl.h>
#include "IClient.h"

class TCP::Client : public TCP::IClient{
public:
    Client() = default;
    Client(uint32_t host, uint16_t port);
    Client(const std::string& IP, uint16_t port);
    int Connect() override;
    int Disconnect() override;
    Buffer LoadData() override;
    int SendData(const std::string& msg);
private:
    // this is the data which will be updated
    std::thread data_loader;
    std::mutex data_mutex;
};