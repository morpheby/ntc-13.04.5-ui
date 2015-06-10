#ifndef MODBUSSHARED_H
#define MODBUSSHARED_H

#include <modbus.h>
#include <mutex>

namespace ModBus {

namespace internal {
struct ModBusShared {
    ModBusShared(modbus_t *modbusCtx) :
        modbusCtx{modbusCtx} {
    }

    modbus_t *modbusCtx;
    std::mutex operationMutex;
};
} /* internal */

} /* ModBus */

#endif // MODBUSSHARED_H

