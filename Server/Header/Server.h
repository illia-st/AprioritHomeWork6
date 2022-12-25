#pragma once
#include "IServer.h"
#include <fcntl.h>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>


class TCP::Server : public TCP::IServer {
    public:
        Server();
        ~Server() = default;
        explicit Server(uint16_t port, KeepAlive if_live = KeepAlive::FALSE);
        void SetNewPort(uint16_t port) override;
        int StartServer() override;
        int StopServer() override;
    protected:
        mutable std::mutex client_mutex;
        mutable std::mutex respond_mutex;
        mutable std::condition_variable respond_cv;
        std::queue<std::pair<std::shared_ptr<CommunicationUnit>, Buffer<>>> responds;
        std::thread acceptor;
        std::thread data_loader;
        std::thread respond_manager;
        [[nodiscard]] const ConnectionsList & GetConnections() const override;
        ConnectionsList & GetConnections() override;
        void AccessingNewConnections() override;
        void LoadData() override;
        void RespondRequests();
    };
