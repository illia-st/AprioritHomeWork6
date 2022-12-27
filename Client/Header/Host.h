#pragma once
#include "../../Common/CommunicationUnit.h"

namespace TCP{
    class Host: public CommunicationUnit{
    public:
        Host();
        Host(uint16_t port, uint32_t host = INADDR_ANY);
        Host(uint16_t port, const std::string& host);
        ~Host() = default;
        Buffer LoadData() override;
    };
}