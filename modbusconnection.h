#ifndef MODBUSCONNECTION_H
#define MODBUSCONNECTION_H

#include <array>

namespace ModBus {

class ModBusConnection
{
public:
    ModBusConnection(void* modbusCtx, int addr);

    template<size_t size>
    std::array<uint16_t, size> readRegisters(int addr) const;
};

} /* ModBus */

#endif // MODBUSCONNECTION_H
