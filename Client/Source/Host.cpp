#include "../Header/Host.h"

TCP::Host::Host(): TCP::CommunicationUnit() {}

TCP::Host::Host(uint16_t port, uint32_t host): TCP::CommunicationUnit(port, host) {}

TCP::Buffer TCP::Host::LoadData(){
    Buffer buffer;
    while(true){
        fd_set rfds;
        struct timeval tv;

        FD_ZERO(&rfds);
        FD_SET(this->GetSocket(), &rfds);
        tv.tv_sec = 5;
        tv.tv_usec = 0;

        int retval = select(this->GetSocket() + 1, &rfds, nullptr, nullptr, &tv);

        if(retval == 0){
            // nothing was received
            continue;
        }
        else if(retval == -1){
            // make log about error
            int error {errno};
            buffer.SetFlag(MsgFlag::Disconnect);
            return std::move(buffer);
        }
       
        socklen_t len {sizeof(this->GetData())};

        int ret = recvfrom(this->GetSocket(), buffer.GetBuffer().get(), Buffer_size, 0,
                           reinterpret_cast<sockaddr*>(&this->GetData()), &len);
        if(ret > 0){
            return std::move(buffer);
        }
        else if(ret == 0){
            // remote was closed
            int error {errno};
            buffer.SetFlag(MsgFlag::Disconnect);
            return std::move(buffer);
        }
        else {
            // something terrible happened
            // Make log about exception
            continue;
        }
    }
}