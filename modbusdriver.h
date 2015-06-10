#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include "modbusconnection.h"

namespace ModBus {

class ModBusDriver
{
public:
    ModBusDriver(const std::string &portLocation);

    ModBusConnection connectionToDevice(int addr) const;
};

} /* ModBus */

#endif // MODBUSDRIVER_H
