#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include "modbusconnection.h"
#include <memory>
#include <string>

namespace ModBus {

namespace internal {
struct ModBusShared;
} /* internal */

class ModBusDriver
{
    std::shared_ptr<internal::ModBusShared> modbusConnection_;
public:
    ModBusDriver(const std::string &portLocation);
    ~ModBusDriver();

    ModBusConnection connectionToDevice(int addr) const;
};

} /* ModBus */

#endif // MODBUSDRIVER_H
