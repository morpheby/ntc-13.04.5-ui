#ifndef MODBUSCONNECTION_HPP
#define MODBUSCONNECTION_HPP

namespace ModBus {

template<size_t size>
std::array<uint16_t, size> ModBusConnection::readRegisters(int addr) {
    std::array<uint16_t, size> data;

    this->modbusReadRegisters(addr, size, data.data());

    return data;
}

template<size_t readSize, size_t writeSize>
std::array<uint16_t, readSize> ModBusConnection::writeAndReadRegisters(int readAddr, int writeAddr,
                                                                       const std::array<uint16_t, writeSize> &values) {
    std::array<uint16_t, readSize> data;

    this->modbusWriteReadRegisters(writeAddr, writeSize, values.data(),
                                   readAddr, readSize, data.data());

    return data;
}


template<size_t readSize, size_t writeSize>
void ModBusConnection::writeRegisters(int addr, const std::array<uint16_t, writeSize> &values) {
    this->modbusWriteRegisters(addr, writeSize, values.data());
}

} /* ModBus */

#endif // MODBUSCONNECTION_HPP

