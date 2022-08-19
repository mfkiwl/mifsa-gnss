#include <mifsa/gnss/platform.h>

MIFSA_NAMESPACE_BEGIN

namespace Gnss {
class PlatformLinux : public Platform {
public:
    virtual std::string getNmea() override
    {
        return "empty";
    }
};

MIFSA_CREATE_PLATFORM(PlatformLinux, 1, 0);
}

MIFSA_NAMESPACE_END
