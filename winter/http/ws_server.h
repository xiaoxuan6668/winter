#ifndef __WINTER_HTTP_WS_SERVER_H__
#define __WINTER_HTTP_WS_SERVER_H__

#include "winter/tcp_server.h"
#include "ws_session.h"
#include "ws_servlet.h"

namespace winter {
namespace http {

class WSServer : public TcpServer {
public:
    typedef std::shared_ptr<WSServer> ptr;

    WSServer(winter::IOManager* worker = winter::IOManager::GetThis()
             , winter::IOManager* accept_worker = winter::IOManager::GetThis());

    WSServletDispatch::ptr getWSServletDispatch() const { return m_dispatch;}
    void setWSServletDispatch(WSServletDispatch::ptr v) { m_dispatch = v;}
protected:
    virtual void handleClient(Socket::ptr client) override;
protected:
    WSServletDispatch::ptr m_dispatch;
};

}
}

#endif