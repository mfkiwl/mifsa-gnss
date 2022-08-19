#ifndef MIFSA_GNSS_TYPES_H
#define MIFSA_GNSS_TYPES_H

#include "mifsa/gnss/config.h"
#include <functional>
#include <string>
#include <mifsa/base/define.h>

MIFSA_NAMESPACE_BEGIN

namespace Gnss {
struct Location {
    int size = 0;
    int flags = 0;
    double latitude = 0;
    double longitude = 0;
    double altitude = 0;
    float speed = 0;
    float bearing = 0;
    float accuracy = 0;
    int64_t timestamp = 0;
    std::string data;
};
using CbNmea = std::function<void(std::string&)>;
using CbLocation = std::function<void(const Location&)>;
using CbStartNavigation = std::function<void()>;
using CbStopNavigation = std::function<void()>;
}

MIFSA_NAMESPACE_END

#endif // MIFSA_GNSS_TYPES_H
