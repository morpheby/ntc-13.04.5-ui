#ifndef MODBUSCONNECTION_H
#define MODBUSCONNECTION_H

#include <array>
#include <mutex>
#include <memory>

namespace ModBus {

namespace internal {
struct ModBusShared;
} /* internal */

class ModBusConnection
{
    std::shared_ptr<internal::ModBusShared> modbusConnection_;
    int devAddr_;
protected:
    void modbusWriteRegisters(int addr, int nb, const uint16_t *src);
    void modbusReadRegisters(int addr, int nb, uint16_t *dest);
    void modbusWriteReadRegisters(int writeAddr, int nbWrite, const uint16_t *src,
                                  int readAddr, int nbRead, uint16_t *dest);

public:
    ModBusConnection(const std::shared_ptr<internal::ModBusShared> &modbusConnection, int addr);

    template<size_t size>
    std::array<uint16_t, size> readRegisters(int addr);

    template<size_t readSize, size_t writeSize>
    std::array<uint16_t, readSize> writeAndReadRegisters(int readAddr, int writeAddr,
                                                         const std::array<uint16_t, writeSize> &values);

    template<size_t readSize, size_t writeSize>
    void writeRegisters(int addr, const std::array<uint16_t, writeSize> &values);

    void flush();
};

} /* ModBus */

#include "modbusconnection.hpp"

#endif // MODBUSCONNECTION_H
