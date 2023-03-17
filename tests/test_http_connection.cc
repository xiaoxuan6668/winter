#include <iostream>
#include "../winter/http/http_connection.h"
#include "../winter/log.h"
#include "../winter/iomanager.h"
#include <fstream>

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

void test_pool() {
    winter::http::HttpConnectionPool::ptr pool(new winter::http::HttpConnectionPool(
                "www.winter.com", "", 80, false, 10, 1000 * 30, 5));

    winter::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 300);
            WINTER_LOG_INFO(g_logger) << r->toString();
    }, true);
}

void run() {
    winter::Address::ptr addr = winter::Address::LookupAnyIPAddress("www.winter.top:80");
    if(!addr) {
        WINTER_LOG_INFO(g_logger) << "get addr error";
        return;
    }

    winter::Socket::ptr sock = winter::Socket::CreateTCP(addr);
    bool rt = sock->connect(addr);
    if(!rt) {
        WINTER_LOG_INFO(g_logger) << "connect " << *addr << " failed";
        return;
    }

    winter::http::HttpConnection::ptr conn(new winter::http::HttpConnection(sock));
    winter::http::HttpRequest::ptr req(new winter::http::HttpRequest);
    req->setPath("/blog/");
    req->setHeader("host", "www.winter.top");
    WINTER_LOG_INFO(g_logger) << "req:" << std::endl
        << *req;

    conn->sendRequest(req);
    auto rsp = conn->recvResponse();

    if(!rsp) {
        WINTER_LOG_INFO(g_logger) << "recv response error";
        return;
    }
    WINTER_LOG_INFO(g_logger) << "rsp:" << std::endl
        << *rsp;

    std::ofstream ofs("rsp.dat");
    ofs << *rsp;

    WINTER_LOG_INFO(g_logger) << "=========================";

    auto r = winter::http::HttpConnection::DoGet("http://www.winter.top/blog/", 300);
    WINTER_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    WINTER_LOG_INFO(g_logger) << "=========================";
    test_pool();
}

void test_https() {
    auto r = winter::http::HttpConnection::DoGet("https://www.baidu.com/", 300);
    WINTER_LOG_INFO(g_logger) << "result=" << r->result
        << " error=" << r->error
        << " rsp=" << (r->response ? r->response->toString() : "");

    //winter::http::HttpConnectionPool::ptr pool(new winter::http::HttpConnectionPool(
    //            "www.baidu.com", "", 443, true, 10, 1000 * 30, 5));
    auto pool = winter::http::HttpConnectionPool::Create(
                    "https://www.baidu.com", "", 10, 1000 * 30, 5);

    winter::IOManager::GetThis()->addTimer(1000, [pool](){
            auto r = pool->doGet("/", 300);
            WINTER_LOG_INFO(g_logger) << r->toString();
    }, true);
}

int main(int argc, char** argv) {
    winter::IOManager iom(2);
    //iom.schedule(run);
    iom.schedule(test_https);
    return 0;
}