#include "../Header/Session.h"

TCP::Session::Session(): TCP::CommunicationUnit() { }

TCP::Session::Session(uint16_t port, uint32_t host): TCP::CommunicationUnit(port, host) { }

TCP::Buffer TCP::Session::LoadData(){
    Buffer buf;
    int ans = recv(this->GetSocket(), buf.GetBuffer().get(), Buffer_size, MSG_DONTWAIT);
    if(ans == -1){
        buf.SetFlag(MsgFlag::EmptyMsg);
        return std::move(buf);
    }
    std::cout << "Something was accepted" << std::endl;
    if(ans == 0){ 
        buf.SetFlag(MsgFlag::Disconnect);
        std::cout << "The client disconnected" << std::endl;
        return std::move(buf);
    }
    buf.SetLastReceivedBytes(ans);
    buf.SetFlag(static_cast<MsgFlag>(buf.GetBuffer().get()[0]));

    return std::move(buf);
}