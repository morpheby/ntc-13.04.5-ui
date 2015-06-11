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
#ifdef DEBUG
    modbus_set_debug(modbusConnection_->modbusCtx, 1);
#endif

    struct timeval response_timeout;
    response_timeout.tv_sec = 0;
    response_timeout.tv_usec = 500000;
    modbus_set_response_timeout(modbusConnection_->modbusCtx, &response_timeout);

    util::Logger::getInstance()->trace("Connecting to modbus");
    int retval = modbus_connect(modbusConnection_->modbusCtx);
    if (retval) {
        throw util::posix_error_exception("connecting to modbus",
                                          MAKE_DEBUG_STRING());
    }
}

ModBusDriver::~ModBusDriver() {
    modbus_close(modbusConnection_->modbusCtx);
}

ModBusConnection ModBusDriver::connectionToDevice(int addr) const {
    util::Logger::getInstance()->trace("Creating ModBusConnection to " + std::to_string(addr));
    return ModBusConnection(modbusConnection_, addr);
}

