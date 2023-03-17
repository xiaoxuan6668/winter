#include "application.h"
#include "winter/config.h"
#include "winter/env.h"
#include "winter/log.h"
#include "winter/daemon.h"
#include "winter/worker.h"
#include "winter/module.h"
#include <unistd.h>

namespace winter {

static winter::Logger::ptr g_logger = WINTER_LOG_NAME("system");

static winter::ConfigVar<std::string>::ptr g_server_work_path =
    winter::Config::Lookup("server.work_path"
            ,std::string("/home/ghx/MyProjects/cpp/winter/bin/work/winter")
            , "server work path");

static winter::ConfigVar<std::string>::ptr g_server_pid_file =
    winter::Config::Lookup("server.pid_file"
            ,std::string("winter.pid")
            , "server pid file");

struct HttpServerConf {
    std::vector<std::string> address;
    int keepalive = 0;
    int timeout = 1000 * 2 * 60;
    int ssl = 0;
    std::string name;
    std::string cert_file;
    std::string key_file;
    std::string accept_worker;
    std::string process_worker;

    bool isValid() const {
        return !address.empty();
    }

    bool operator==(const HttpServerConf& oth) const {
        return address == oth.address
            && keepalive == oth.keepalive
            && timeout == oth.timeout
            && name == oth.name
            && ssl == oth.ssl
            && cert_file == oth.cert_file
            && key_file == oth.key_file
            && accept_worker == oth.accept_worker
            && process_worker == oth.process_worker;
    }
};

template<>
class LexicalCast<std::string, HttpServerConf> {
public:
    HttpServerConf operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        HttpServerConf conf;
        conf.keepalive = node["keepalive"].as<int>(conf.keepalive);
        conf.timeout = node["timeout"].as<int>(conf.timeout);
        conf.name = node["name"].as<std::string>(conf.name);
        conf.ssl = node["ssl"].as<int>(conf.ssl);
        conf.cert_file = node["cert_file"].as<std::string>(conf.cert_file);
        conf.key_file = node["key_file"].as<std::string>(conf.key_file);
        conf.accept_worker = node["accept_worker"].as<std::string>();
        conf.process_worker = node["process_worker"].as<std::string>();
        if(node["address"].IsDefined()) {
            for(size_t i = 0; i < node["address"].size(); ++i) {
                conf.address.push_back(node["address"][i].as<std::string>());
            }
        }
        return conf;
    }
};

template<>
class LexicalCast<HttpServerConf, std::string> {
public:
    std::string operator()(const HttpServerConf& conf) {
        YAML::Node node;
        node["name"] = conf.name;
        node["keepalive"] = conf.keepalive;
        node["timeout"] = conf.timeout;
        node["ssl"] = conf.ssl;
        node["cert_file"] = conf.cert_file;
        node["key_file"] = conf.key_file;
        node["accept_worker"] = conf.accept_worker;
        node["process_worker"] = conf.process_worker;
        for(auto& i : conf.address) {
            node["address"].push_back(i);
        }
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

static winter::ConfigVar<std::vector<HttpServerConf> >::ptr g_http_servers_conf
    = winter::Config::Lookup("http_servers", std::vector<HttpServerConf>(), "http server config");

Application* Application::s_instance = nullptr;

Application::Application() {
    s_instance = this;
}

bool Application::init(int argc, char** argv) {
    m_argc = argc;
    m_argv = argv;

    winter::EnvMgr::GetInstance()->addHelp("s", "start with the terminal");
    winter::EnvMgr::GetInstance()->addHelp("d", "run as daemon");
    winter::EnvMgr::GetInstance()->addHelp("c", "conf path default: ./conf");
    winter::EnvMgr::GetInstance()->addHelp("p", "print help");

    bool is_print_help = false;
    if(!winter::EnvMgr::GetInstance()->init(argc, argv)) {
        is_print_help = true;
    }

    if(winter::EnvMgr::GetInstance()->has("p")) {
        is_print_help = true;
    }

    std::string conf_path = winter::EnvMgr::GetInstance()->getConfigPath();
    WINTER_LOG_INFO(g_logger) << "load conf path:" << conf_path;
    winter::Config::LoadFromConfDir(conf_path);

    ModuleMgr::GetInstance()->init();
    std::vector<Module::ptr> modules;
    ModuleMgr::GetInstance()->listAll(modules);

    for(auto i : modules) {
        i->onBeforeArgsParse(argc, argv);
    }

    if(is_print_help) {
        winter::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    for(auto i : modules) {
        i->onAfterArgsParse(argc, argv);
    }
    modules.clear();

    int run_type = 0;
    if(winter::EnvMgr::GetInstance()->has("s")) {
        run_type = 1;
    }
    if(winter::EnvMgr::GetInstance()->has("d")) {
        run_type = 2;
    }

    if(run_type == 0) {
        winter::EnvMgr::GetInstance()->printHelp();
        return false;
    }

    std::string conf_path = winter::EnvMgr::GetInstance()->getAbsolutePath(
                winter::EnvMgr::GetInstance()->get("c", "conf")
                );
    WINTER_LOG_INFO(g_logger) << "load conf path:" << conf_path;
    winter::Config::LoadFromConfDir(conf_path);

    std::string pidfile = g_server_work_path->getValue()
                                + "/" + g_server_pid_file->getValue();
    if(winter::FSUtil::IsRunningPidfile(pidfile)) {
        WINTER_LOG_ERROR(g_logger) << "server is running:" << pidfile;
        return false;
    }

    if(!winter::FSUtil::Mkdir(g_server_work_path->getValue())) {
        WINTER_LOG_FATAL(g_logger) << "create work path [" << g_server_work_path->getValue()
            << " errno=" << errno << " errstr=" << strerror(errno);
        return false;
    }
    return true;
}

bool Application::run() {
    bool is_daemon = winter::EnvMgr::GetInstance()->has("d");
    return start_daemon(m_argc, m_argv,
            std::bind(&Application::main, this, std::placeholders::_1,
                std::placeholders::_2), is_daemon);
}

int Application::main(int argc, char** argv) {
    WINTER_LOG_INFO(g_logger) << "main";
    {
        std::string pidfile = g_server_work_path->getValue()
                                    + "/" + g_server_pid_file->getValue();
        std::ofstream ofs(pidfile);
        if(!ofs) {
            WINTER_LOG_ERROR(g_logger) << "open pidfile " << pidfile << " failed";
            return false;
        }
        ofs << getpid();
    }

    m_mainIOManager.reset(new winter::IOManager(1, true, "main"));
    m_mainIOManager->schedule(std::bind(&Application::run_fiber, this));
    m_mainIOManager->addTimer(1000, [](){}, true);
    m_mainIOManager->stop();
    return 0;
}

int Application::run_fiber() {
    winter::WorkerMgr::GetInstance()->init();
    auto http_confs = g_http_servers_conf->getValue();
    for(auto& i : http_confs) {
        WINTER_LOG_INFO(g_logger) << LexicalCast<HttpServerConf, std::string>()(i);

        std::vector<Address::ptr> address;
        for(auto& a : i.address) {
            size_t pos = a.find(":");
            if(pos == std::string::npos) {
                //WINTER_LOG_ERROR(g_logger) << "invalid address: " << a;
                address.push_back(UnixAddress::ptr(new UnixAddress(a)));
                continue;
            }
            int32_t port = atoi(a.substr(pos + 1).c_str());
            //127.0.0.1
            auto addr = winter::IPAddress::Create(a.substr(0, pos).c_str(), port);
            if(addr) {
                address.push_back(addr);
                continue;
            }
            std::vector<std::pair<Address::ptr, uint32_t> > result;
            if(winter::Address::GetInterfaceAddresses(result,
                                        a.substr(0, pos))) {
                for(auto& x : result) {
                    auto ipaddr = std::dynamic_pointer_cast<IPAddress>(x.first);
                    if(ipaddr) {
                        ipaddr->setPort(atoi(a.substr(pos + 1).c_str()));
                    }
                    address.push_back(ipaddr);
                }
                continue;
            }

            auto aaddr = winter::Address::LookupAny(a);
            if(aaddr) {
                address.push_back(aaddr);
                continue;
            }
            WINTER_LOG_ERROR(g_logger) << "invalid address: " << a;
            _exit(0);
        }
        IOManager* accept_worker = winter::IOManager::GetThis();
        IOManager* process_worker = winter::IOManager::GetThis();
        if(!i.accept_worker.empty()) {
            accept_worker = winter::WorkerMgr::GetInstance()->getAsIOManager(i.accept_worker).get();
            if(!accept_worker) {
                WINTER_LOG_ERROR(g_logger) << "accept_worker: " << i.accept_worker
                    << " not exists";
                _exit(0);
            }
        }
        if(!i.process_worker.empty()) {
            process_worker = winter::WorkerMgr::GetInstance()->getAsIOManager(i.process_worker).get();
            if(!process_worker) {
                WINTER_LOG_ERROR(g_logger) << "process_worker: " << i.process_worker
                    << " not exists";
                _exit(0);
            }
        }

        winter::http::HttpServer::ptr server(new winter::http::HttpServer(i.keepalive,
                    process_worker, accept_worker));
        std::vector<Address::ptr> fails;
        if(!server->bind(address, fails, i.ssl)) {
            for(auto& x : fails) {
                WINTER_LOG_ERROR(g_logger) << "bind address fail:"
                    << *x;
            }
            _exit(0);
        }
        if(i.ssl) {
            if(!server->loadCertificates(i.cert_file, i.key_file)) {
                WINTER_LOG_ERROR(g_logger) << "loadCertificates fail, cert_file="
                    << i.cert_file << " key_file=" << i.key_file;
            }
        }
        if(!i.name.empty()) {
            server->setName(i.name);
        }
        server->start();
        m_httpservers.push_back(server);

    }
    return 0;
}

}