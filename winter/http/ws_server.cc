#include "ws_server.h"
#include "winter/log.h"

namespace winter {
namespace http {

static winter::Logger::ptr g_logger = WINTER_LOG_NAME("system");

WSServer::WSServer(winter::IOManager* worker, winter::IOManager* accept_worker)
    :TcpServer(worker, accept_worker) {
    m_dispatch.reset(new WSServletDispatch);
}

void WSServer::handleClient(Socket::ptr client) {
    WINTER_LOG_DEBUG(g_logger) << "handleClient " << *client;
    WSSession::ptr session(new WSSession(client));
    do {
        HttpRequest::ptr header = session->handleShake();
        if(!header) {
            WINTER_LOG_DEBUG(g_logger) << "handleShake error";
            break;
        }
        WSServlet::ptr servlet = m_dispatch->getWSServlet(header->getPath());
        if(!servlet) {
            WINTER_LOG_DEBUG(g_logger) << "no match WSServlet";
            break;
        }
        int rt = servlet->onConnect(header, session);
        if(rt) {
            WINTER_LOG_DEBUG(g_logger) << "onConnect return " << rt;
            break;
        }
        while(true) {
            auto msg = session->recvMessage();
            if(!msg) {
                break;
            }
            rt = servlet->handle(header, msg, session);
            if(rt) {
                WINTER_LOG_DEBUG(g_logger) << "handle return " << rt;
                break;
            }
        }
        servlet->onClose(header, session);
    } while(0);
    session->close();
}

}
}