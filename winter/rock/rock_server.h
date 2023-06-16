#ifndef __WINTER_ROCK_SERVER_H__
#define __WINTER_ROCK_SERVER_H__

#include "winter/rock/rock_stream.h"
#include "winter/tcp_server.h"

namespace winter {

class RockServer : public TcpServer {
public:
    typedef std::shared_ptr<RockServer> ptr;
    RockServer(winter::IOManager* worker = winter::IOManager::GetThis()
               ,winter::IOManager* accept_worker = winter::IOManager::GetThis());

protected:
    virtual void handleClient(Socket::ptr client) override;
};

}

#endif
