#include "modbusconnection.h"
#include "modbusshared.h"
#include "serial/Logger.h"

using namespace ModBus;
using namespace ModBus::internal;

ModBusConnection::ModBusConnection(const std::shared_ptr<ModBusShared> &modbusConnection, int addr) :
    modbusConnection_{modbusConnection},
    devAddr_{addr} {

    util::Logger::getInstance()->trace("ModBusConnection to " + std::to_string(addr) + " created");
}

void ModBusConnection::modbusWriteRegisters(int addr, int nb, const uint16_t *src) {
    util::Logger::getInstance()->trace("RegisterWrite to " +
                                       std::to_string(devAddr_) + ":" + std::to_string(addr));
    util::Logger::getInstance()->log("Communicating with device " + std::to_string(devAddr_));

    std::lock_guard<std::mutex> lock(modbusConnection_->operationMutex);

    int retval = modbus_set_slave(modbusConnection_->modbusCtx, devAddr_);
    if (retval) {
        throw util::posix_error_exception("setting slave address", MAKE_DEBUG_STRING());
    }

    util::Logger::getInstance()->trace("Writing data to " +
                                       std::to_string(devAddr_) + ":" + std::to_string(addr));
    retval = modbus_write_registers(modbusConnection_->modbusCtx, addr, nb, src);
    if (retval != nb) {
        throw util::posix_error_exception("writing data to " +
                                          std::to_string(devAddr_) + ":" + std::to_string(addr),
                                          MAKE_DEBUG_STRING());
    }
    util::Logger::getInstance()->trace("Data write completed");
}

void ModBusConnection::modbusReadRegisters(int addr, int nb, uint16_t *dest) {
    util::Logger::getInstance()->trace("RegisterRead from " +
                                       std::to_string(devAddr_) + ":" + std::to_string(addr));
    util::Logger::getInstance()->log("Communicating with device " + std::to_string(devAddr_));

    std::lock_guard<std::mutex> lock(modbusConnection_->operationMutex);

    int retval = modbus_set_slave(modbusConnection_->modbusCtx, devAddr_);
    if (retval) {
        throw util::posix_error_exception("setting slave address", MAKE_DEBUG_STRING());
    }

    util::Logger::getInstance()->trace("Reading data from " +
                                       std::to_string(devAddr_) + ":" + std::to_string(addr));

    retval = modbus_read_registers(modbusConnection_->modbusCtx, addr, nb, dest);
    if (retval != nb) {
        throw util::posix_error_exception("Reading data from " +
                                          std::to_string(devAddr_) + ":" + std::to_string(addr),
                                          MAKE_DEBUG_STRING());
    }
    util::Logger::getInstance()->trace("Data read completed");
}

void ModBusConnection::modbusWriteReadRegisters(int writeAddr, int nbWrite, const uint16_t *src,
                                                int readAddr, int nbRead, uint16_t *dest) {
    std::lock_guard<std::mutex> lock(modbusConnection_->operationMutex);

    modbus_set_slave(modbusConnection_->modbusCtx, devAddr_);

    modbus_write_and_read_registers(modbusConnection_->modbusCtx,
                                    writeAddr, nbWrite, src,
                                    readAddr, nbRead, dest);
}

void ModBusConnection::flush() {
    util::Logger::getInstance()->log("Flushing connection");
    int retval = modbus_flush(modbusConnection_->modbusCtx);

    if (retval) {
        throw util::posix_error_exception("Flushing connection",
                                          MAKE_DEBUG_STRING());
    }
}
