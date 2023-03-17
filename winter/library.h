#ifndef __WINTER_LIBRARY_H__
#define __WINTER_LIBRARY_H__

#include <memory>
#include "module.h"

namespace winter {

class Library {
public:
    static Module::ptr GetModule(const std::string& path);
};

}

#endif