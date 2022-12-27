#pragma once
#include <memory>
#include "Core.h"

namespace TCP{
    enum class MsgFlag: uint8_t{
        EmptyMsg = 1,
        GreetingMsg,
        EchoMsg,
        Disconnect = 100
    };    

    class Buffer{
    public:
        Buffer(): last_received_bytes(0), buffer(std::make_shared<char[]>(Buffer_size)), flag(MsgFlag::EmptyMsg) {}
        Buffer(Buffer&& other): last_received_bytes(other.last_received_bytes), buffer(std::move(other.buffer)), flag(other.flag){
            other.flag = MsgFlag::Disconnect;
            other.last_received_bytes = 0;
        }
        Buffer& operator = (Buffer&& other){
            last_received_bytes = other.last_received_bytes;
            buffer = std::move(other.buffer);
            flag = other.flag;
            other.flag = MsgFlag::Disconnect;
            other.last_received_bytes = 0;
            return *this;
        }
        std::shared_ptr<char[]>& GetBuffer(){
            return buffer;
        }
        const std::shared_ptr<char[]>& GetBuffer() const{
            return buffer;
        }
        MsgFlag GetFlag() const{
            return flag;
        }
        void SetFlag(MsgFlag msg_flag){
            flag = msg_flag;
        }
        std::size_t GetLastReceivedBytes() const{
            return last_received_bytes;
        }
        void SetLastReceivedBytes(std::size_t bytes){
            last_received_bytes = bytes;
        }
    private:
        std::size_t last_received_bytes;
        std::shared_ptr<char[]> buffer;
        MsgFlag flag;
    };
}