#include "modbusdriver.h"
#include "modbusshared.h"
#include <modbus.h>
#include "serial/Logger.h"

using namespace ModBus;

ModBusDriver::ModBusDriver(const std::string &portLocation) :
    modbusConnection_(new internal::ModBusShared(
            modbus_new_rtu(portLocation.c_str(), 19200, 'N', 8, 1)),
        [](internal::ModBusShared *m){
            std::lock_guard<std::mutex> lock(m->operationMutex);
            modbus_free(m->modbusCtx);
            delete m;
        } ) {
    if (!modbusConnection_->modbusCtx) {
        throw util::posix_error_exception("initializing ModBusDriver",
                                          MAKE_DEBUG_STRING());
    }
    util::Logger::getInstance()->trace("ModBusDriver created");
}


ModBusConnection ModBusDriver::connectionToDevice(int addr) const {
    util::Logger::getInstance()->trace("Creating ModBusConnection to " + std::to_string(addr));
    return ModBusConnection(modbusConnection_, addr);
}

