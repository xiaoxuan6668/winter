#include "winter/http/ws_server.h"
#include "winter/log.h"

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void run() {
    winter::http::WSServer::ptr server(new winter::http::WSServer);
    winter::Address::ptr addr = winter::Address::LookupAnyIPAddress("0.0.0.0:8020");
    if(!addr) {
        WINTER_LOG_ERROR(g_logger) << "get address error";
        return;
    }
    auto fun = [](winter::http::HttpRequest::ptr header
                  ,winter::http::WSFrameMessage::ptr msg
                  ,winter::http::WSSession::ptr session) {
        session->sendMessage(msg);
        return 0;
    };


    server->getWSServletDispatch()->addServlet("/winter", fun);
    while(!server->bind(addr)) {
        WINTER_LOG_ERROR(g_logger) << "bind " << *addr << " fail";
        sleep(1);
    }
    server->start();
}

int main(int argc, char** argv) {
    winter::IOManager iom(2);
    iom.schedule(run);
    return 0;
}