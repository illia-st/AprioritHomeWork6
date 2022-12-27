#pragma once
#include "../../Common/CommunicationUnit.h"
#include <iostream>

namespace TCP{
    class Session: public CommunicationUnit{
    public:
        Session();
        Session(uint16_t port, uint32_t host = INADDR_ANY);
        ~Session() = default;
        Buffer LoadData() override;
    };
}