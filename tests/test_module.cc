#include "winter/module.h"
#include "winter/singleton.h"
#include <iostream>

class A {
public:
    A() {
        std::cout << "A::A " << this << std::endl;
    }

    ~A() {
        std::cout << "A::~A " << this << std::endl;
    }

};

class MyModule : public winter::Module {
public:
    MyModule()
        :Module("hello", "1.0", "") {
        //winter::Singleton<A>::GetInstance();
    }

    bool onLoad() override {
        winter::Singleton<A>::GetInstance();
        std::cout << "-----------onLoad------------" << std::endl;
        return true;
    }

    bool onUnload() override {
        winter::Singleton<A>::GetInstance();
        std::cout << "-----------onUnload------------" << std::endl;
        return true;
    }
};

extern "C" {

winter::Module* CreateModule() {
    winter::Singleton<A>::GetInstance();
    std::cout << "=============CreateModule=================" << std::endl;
    return new MyModule;
}

void DestoryModule(winter::Module* ptr) {
    std::cout << "=============DestoryModule=================" << std::endl;
    delete ptr;
}

}