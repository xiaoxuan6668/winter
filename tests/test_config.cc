#include "../winter/config.h"
#include "../winter/log.h"
#include <yaml-cpp/yaml.h>
#include "winter/env.h"
#include <iostream>

#if 0
winter::ConfigVar<int>::ptr g_int_value_config =
    winter::Config::Lookup("system.port", (int)8080, "system port");

winter::ConfigVar<float>::ptr g_int_valuex_config =
    winter::Config::Lookup("system.port", (float)8080, "system port");

winter::ConfigVar<float>::ptr g_float_value_config =
    winter::Config::Lookup("system.value", (float)10.2f, "system value");

winter::ConfigVar<std::vector<int> >::ptr g_int_vec_value_config =
    winter::Config::Lookup("system.int_vec", std::vector<int>{1,2}, "system int vec");

winter::ConfigVar<std::list<int> >::ptr g_int_list_value_config =
    winter::Config::Lookup("system.int_list", std::list<int>{1,2}, "system int list");

winter::ConfigVar<std::set<int> >::ptr g_int_set_value_config =
    winter::Config::Lookup("system.int_set", std::set<int>{1,2}, "system int set");

winter::ConfigVar<std::unordered_set<int> >::ptr g_int_uset_value_config =
    winter::Config::Lookup("system.int_uset", std::unordered_set<int>{1,2}, "system int uset");

winter::ConfigVar<std::map<std::string, int> >::ptr g_str_int_map_value_config =
    winter::Config::Lookup("system.str_int_map", std::map<std::string, int>{{"k",2}}, "system str int map");

winter::ConfigVar<std::unordered_map<std::string, int> >::ptr g_str_int_umap_value_config =
    winter::Config::Lookup("system.str_int_umap", std::unordered_map<std::string, int>{{"k",2}}, "system str int map");

void print_yaml(const YAML::Node& node, int level) {
    if(node.IsScalar()) {
        WINTER_LOG_INFO(WINTER_LOG_ROOT()) << std::string(level * 4, ' ')
            << node.Scalar() << " - " << node.Type() << " - " << level;
    } else if(node.IsNull()) {
        WINTER_LOG_INFO(WINTER_LOG_ROOT()) << std::string(level * 4, ' ')
            << "NULL - " << node.Type() << " - " << level;
    } else if(node.IsMap()) {
        for(auto it = node.begin();
                it != node.end(); ++it) {
            WINTER_LOG_INFO(WINTER_LOG_ROOT()) << std::string(level * 4, ' ')
                    << it->first << " - " << it->second.Type() << " - " << level;
            print_yaml(it->second, level + 1);
        }
    } else if(node.IsSequence()) {
        for(size_t i = 0; i < node.size(); ++i) {
            WINTER_LOG_INFO(WINTER_LOG_ROOT()) << std::string(level * 4, ' ')
                << i << " - " << node[i].Type() << " - " << level;
            print_yaml(node[i], level + 1);
        }
    }
}

void test_yaml() {
    YAML::Node root = YAML::LoadFile("/home/ghx/MyProjects/cpp/winter/bin/conf/test.yml");
    print_yaml(root, 0);

    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << root.Scalar();
}

void test_config() {
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "before: " << g_int_value_config->getValue();
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "before: " << g_float_value_config->toString();

#define XX(g_var, name, prefix) \
    {\
        auto& v = g_var->getValue(); \
        for(auto& i : v){   \
            WINTER_LOG_INFO(WINTER_LOG_ROOT()) << #prefix " " #name ": " << i;  \
        }\
    }

#define XX_M(g_var, name, prefix) \
    { \
        auto& v = g_var->getValue(); \
        for(auto& i : v) { \
            WINTER_LOG_INFO(WINTER_LOG_ROOT()) << #prefix " " #name ": {" \
                    << i.first << " - " << i.second << "}"; \
        } \
        WINTER_LOG_INFO(WINTER_LOG_ROOT()) << #prefix " " #name " yaml: " << g_var->toString(); \
    }

    XX(g_int_vec_value_config, int_vec, before);
    XX(g_int_list_value_config, int_list, before);
    XX(g_int_set_value_config, int_set, before);
    XX(g_int_uset_value_config, int_uset, before);
    XX_M(g_str_int_map_value_config, str_int_map, before);
    XX_M(g_str_int_umap_value_config, str_int_umap, before);

    YAML::Node root = YAML::LoadFile("/home/ghx/MyProjects/cpp/winter/bin/conf/test.yml");
    winter::Config::LoadFromYaml(root);

    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "after: " << g_int_value_config->getValue();
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "after: " << g_float_value_config->toString();
    
    XX(g_int_vec_value_config, int_vec, after);
    XX(g_int_list_value_config, int_list, after);
    XX(g_int_set_value_config, int_set, after);
    XX(g_int_uset_value_config, int_uset, after);
    XX_M(g_str_int_map_value_config, str_int_map, after);
    XX_M(g_str_int_umap_value_config, str_int_umap, after);
}

#endif

class Person {
public:
    Person() {};
    std::string m_name;
    int m_age = 0;
    bool m_sex = 0;

    std::string toString() const {
        std::stringstream ss;
        ss << "[Person name=" << m_name
           << " age=" << m_age
           << " sex=" << m_sex
           << "]";
        return ss.str();
    }

    bool operator==(const Person oth) const{
        return m_name == oth.m_name && m_age == oth.m_age 
                && m_sex == oth.m_sex;
    }

};


namespace winter {

template<>
class LexicalCast<std::string, Person> {
public:
    Person operator()(const std::string& v) {
        YAML::Node node = YAML::Load(v);
        Person p;
        p.m_name = node["name"].as<std::string>();
        p.m_age = node["age"].as<int>();
        p.m_sex = node["sex"].as<bool>();
        return p;
    }
};

template<>
class LexicalCast<Person, std::string> {
public:
    std::string operator()(const Person& p) {
        YAML::Node node;
        node["name"] = p.m_name;
        node["age"] = p.m_age;
        node["sex"] = p.m_sex;
        std::stringstream ss;
        ss << node;
        return ss.str();
    }
};

}

winter::ConfigVar<Person>::ptr g_person =
    winter::Config::Lookup("class.person", Person(), "system person");

winter::ConfigVar<std::map<std::string, Person> >::ptr g_person_map =
    winter::Config::Lookup("class.map", std::map<std::string, Person>(), "system person");

winter::ConfigVar<std::map<std::string, std::vector<Person> > >::ptr g_person_vec_map =
    winter::Config::Lookup("class.vec_map", std::map<std::string, std::vector<Person> >(), "system person");


void test_class(){
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "before: " << g_person->getValue().toString()
    << " - " << g_person->toString();

    #define XX_PM(g_var, prefix) \
    { \
        auto m = g_person_map->getValue(); \
        for(auto& i : m) { \
            WINTER_LOG_INFO(WINTER_LOG_ROOT()) <<  prefix << ": " << i.first << " - " << i.second.toString(); \
        } \
        WINTER_LOG_INFO(WINTER_LOG_ROOT()) <<  prefix << ": size=" << m.size(); \
    }

    g_person->addListener([](const Person& old_value,const Person& new_value){
        WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "old_value=" << old_value.toString()
                << " new_value=" << new_value.toString();
    });

    XX_PM(g_person_map, "class.map before");
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "before: " << g_person_vec_map->toString();

    YAML::Node root = YAML::LoadFile("/home/ghx/MyProjects/cpp/winter/bin/conf/test.yml");
    winter::Config::LoadFromYaml(root);

    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "after: " << g_person->getValue().toString()
    << " - " << g_person->toString();
    XX_PM(g_person_map, "class.map after");
    WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "after: " << g_person_vec_map->toString();

}

void test_log() {
    static winter::Logger::ptr system_log = WINTER_LOG_NAME("system");
    WINTER_LOG_INFO(system_log) << "hello system" << std::endl;
    std::cout << winter::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    YAML::Node root = YAML::LoadFile("/home/ghx/MyProjects/cpp/winter/bin/conf/log.yml");
    winter::Config::LoadFromYaml(root);
    std::cout << "=============" << std::endl;
    std::cout << winter::LoggerMgr::GetInstance()->toYamlString() << std::endl;
    std::cout << "=============" << std::endl;
    std::cout << root << std::endl;
    WINTER_LOG_INFO(system_log) << "hello system" << std::endl;

    system_log->setFormatter("%d - %m%n");
    WINTER_LOG_INFO(system_log) << "hello system" << std::endl;
}

void test_loadconf() {
    winter::Config::LoadFromConfDir("conf");
}

int main(int argc, char** argv) {
    // test_yaml();
    // test_config();
    // test_class();
    //test_log();
    winter::EnvMgr::GetInstance()->init(argc, argv);
    test_loadconf();
    std::cout << " ==== " << std::endl;
    // sleep(5);
    // test_loadconf();
    return 0;

    // winter::Config::Visit([](winter::ConfigVarBase::ptr var) {
    //     WINTER_LOG_INFO(WINTER_LOG_ROOT()) << "name=" << var->getName()
    //                 << " description=" << var->getDescription()
    //                 << " typename=" << var->getTypeName()
    //                 << " value=" << var->toString();
    // });

    // return 0;
}