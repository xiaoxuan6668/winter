#include "../winter/http/http_server.h"
#include "../winter/log.h"

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

#define XX(...) #__VA_ARGS__

winter::IOManager::ptr worker;
void run() {
    g_logger->setLevel(winter::LogLevel::INFO);
    //winter::http::HttpServer::ptr server(new winter::http::HttpServer(true, worker.get(), winter::IOManager::GetThis()));
    winter::http::HttpServer::ptr server(new winter::http::HttpServer(true));
    winter::Address::ptr addr = winter::Address::LookupAnyIPAddress("0.0.0.0:8020");
    while(!server->bind(addr)) {
        sleep(2);
    }
    auto sd = server->getServletDispatch();
    sd->addServlet("/winter/xx", [](winter::http::HttpRequest::ptr req
                ,winter::http::HttpResponse::ptr rsp
                ,winter::http::HttpSession::ptr session) {
            rsp->setBody(req->toString());
            return 0;
    });

    sd->addGlobServlet("/winter/*", [](winter::http::HttpRequest::ptr req
                ,winter::http::HttpResponse::ptr rsp
                ,winter::http::HttpSession::ptr session) {
            rsp->setBody("Glob:\r\n" + req->toString());
            return 0;
    });

    sd->addGlobServlet("/winterx/*", [](winter::http::HttpRequest::ptr req
                ,winter::http::HttpResponse::ptr rsp
                ,winter::http::HttpSession::ptr session) {
            rsp->setBody(XX(<html>
<head><title>404 Not Found</title></head>
<body>
<center><h1>404 Not Found</h1></center>
<hr><center>nginx/1.16.0</center>
</body>
</html>
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
<!-- a padding to disable MSIE and Chrome friendly error page -->
));
            return 0;
    });

    server->start();
}

int main(int argc, char** argv) {
    winter::IOManager iom(1, true, "main");
    worker.reset(new winter::IOManager(3, false, "worker"));
    iom.schedule(run);
    return 0;
}