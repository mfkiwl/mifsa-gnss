#ifndef MIFSA_GNSS_PLATFORM_H
#define MIFSA_GNSS_PLATFORM_H

#include <mifsa/module/platform.hpp>

MIFSA_NAMESPACE_BEGIN

namespace Gnss {
class Platform : public PlatformBase {
    MIFSA_PLUGIN_REGISTER("mifsa_gnss_platform")
public:
    virtual std::string getNmea() = 0;
};
}

MIFSA_NAMESPACE_END

#endif // MIFSA_GNSS_PLATFORM_H
