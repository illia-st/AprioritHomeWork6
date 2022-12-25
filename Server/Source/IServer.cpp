#include "../Header/IServer.h"

TCP::IServer::IServer(): info(ServerInfo()), connections(){}
TCP::IServer::IServer(uint16_t port, KeepAlive if_live) {
    alive = if_live;
    info = ServerInfo(port);
}
TCP::IServer::~IServer(){connections.clear();}
const TCP::ConnectionsList &TCP::IServer::GetConnections() const {
    return connections;
}

TCP::ConnectionsList &TCP::IServer::GetConnections() {
    return connections;
}

const TCP::ServerInfo &TCP::IServer::GetInfo() const {return info;}

TCP::ServerInfo &TCP::IServer::GetInfo() {return info;}

void TCP::IServer::SetAliveStatus(KeepAlive if_live) {alive = if_live;}

TCP::KeepAlive TCP::IServer::GetAliveStatus() const {return alive;}

uint16_t TCP::IServer::GetPort() const {return info.GetPort(); }