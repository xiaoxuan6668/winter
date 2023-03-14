#include "../winter/uri.h"
#include <iostream>

int main(int argc, char** argv) {
    //winter::Uri::ptr uri = winter::Uri::Create("http://www.winter.top/test/uri?id=100&name=winter#frg");
    //winter::Uri::ptr uri = winter::Uri::Create("http://admin@www.winter.top/test/中文/uri?id=100&name=winter&vv=中文#frg中文");
    winter::Uri::ptr uri = winter::Uri::Create("http://admin@www.winter.top");
    //winter::Uri::ptr uri = winter::Uri::Create("http://www.winter.top/test/uri");
    std::cout << uri->toString() << std::endl;
    auto addr = uri->createAddress();
    std::cout << *addr << std::endl;
    return 0;
}