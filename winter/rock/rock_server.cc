#include "rock_server.h"
#include "winter/log.h"
#include "winter/module.h"

namespace winter {

static winter::Logger::ptr g_logger = WINTER_LOG_NAME("system");

RockServer::RockServer(winter::IOManager* worker
                       ,winter::IOManager* accept_worker)
    :TcpServer(worker, accept_worker) {
    m_type = "rock";
}

void RockServer::handleClient(Socket::ptr client) {
    WINTER_LOG_DEBUG(g_logger) << "handleClient " << *client;
    winter::RockSession::ptr session(new winter::RockSession(client));
    ModuleMgr::GetInstance()->foreach(Module::ROCK,
            [session](Module::ptr m) {
        m->onConnect(session);
    });
    session->setDisconnectCb(
        [](AsyncSocketStream::ptr stream) {
             ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [stream](Module::ptr m) {
                m->onDisconnect(stream);
            });
        }
    );
    session->setRequestHandler(
        [](winter::RockRequest::ptr req
           ,winter::RockResponse::ptr rsp
           ,winter::RockStream::ptr conn)->bool {
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, req, rsp, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                auto rm = std::dynamic_pointer_cast<RockModule>(m);
                if(rm) {
                    rt = rm->handle(req, rsp, conn);
                }
            });
            return rt;
        }
    ); 
    session->setNotifyHandler(
        [](winter::RockNotify::ptr nty
           ,winter::RockStream::ptr conn)->bool {
            bool rt = false;
            ModuleMgr::GetInstance()->foreach(Module::ROCK,
                    [&rt, nty, conn](Module::ptr m) {
                if(rt) {
                    return;
                }
                auto rm = std::dynamic_pointer_cast<RockModule>(m);
                if(rm) {
                    rt = rm->handle(nty, conn);
                }
            });
            return rt;
        }
    );
    session->start();
}

}