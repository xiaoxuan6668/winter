#include "my_module.h"
#include "winter/config.h"
#include "winter/log.h"

namespace name_space {

static winter::Logger::ptr g_logger = WINTER_LOG_ROOT();

MyModule::MyModule()
    :winter::Module("project_name", "1.0", "") {
}

bool MyModule::onLoad() {
    WINTER_LOG_INFO(g_logger) << "onLoad";
    return true;
}

bool MyModule::onUnload() {
    WINTER_LOG_INFO(g_logger) << "onUnload";
    return true;
}

bool MyModule::onServerReady() {
    WINTER_LOG_INFO(g_logger) << "onServerReady";
    return true;
}

bool MyModule::onServerUp() {
    WINTER_LOG_INFO(g_logger) << "onServerUp";
    return true;
}

}

extern "C" {

winter::Module* CreateModule() {
    winter::Module* module = new name_space::MyModule;
    WINTER_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    return module;
}

void DestoryModule(winter::Module* module) {
    WINTER_LOG_INFO(name_space::g_logger) << "CreateModule " << module;
    delete module;
}

}