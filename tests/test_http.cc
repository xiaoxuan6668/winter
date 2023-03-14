#include "../winter/http/http.h"
#include "../winter/log.h"

void test_request() {
    winter::http::HttpRequest::ptr req(new winter::http::HttpRequest);
    req->setHeader("host" , "www.winter.top");
    req->setBody("hello winter");
    req->dump(std::cout) << std::endl;
}

void test_response() {
    winter::http::HttpResponse::ptr rsp(new winter::http::HttpResponse);
    rsp->setHeader("X-X", "winter");
    rsp->setBody("hello winter");
    rsp->setStatus((winter::http::HttpStatus)400);
    rsp->setClose(false);

    rsp->dump(std::cout) << std::endl;
}

int main(int argc, char** argv) {
    test_request();
    test_response();
    return 0;
}