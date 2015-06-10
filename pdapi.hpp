#ifndef PDAPI_HPP
#define PDAPI_HPP

#include <array>
#include "serial/Logger.h"

namespace PD {

template<typename T>
T PdApi::readRegister(const RegisterInfo<T>& registerInfo) {
    if (registerInfo.size == 1) {
        std::array<uint16_t, 1> data = connection_.readRegisters<1>(registerInfo.address);
        return T(*((T*)data.data()));
    } else if (registerInfo.size == 2) {
        std::array<uint16_t, 2> data = connection_.readRegisters<2>(registerInfo.address);
        return T(*((T*)data.data()));
    } else {
        throw util::info_exception("Unsupported register size encountered",
                                   MAKE_DEBUG_STRING());
    }
}

template<typename T>
void PdApi::writeRegister(const RegisterInfo<T>& registerInfo, const T& value) {
    if (registerInfo.size == 1) {
        std::array<uint16_t, 1> data;
        *(data.data()) = (T*) &value;
        connection_.writeRegisters(registerInfo.address, data);
    } else if (registerInfo.size == 2) {
        std::array<uint16_t, 2> data;
        *(data.data()) = (T*) &value;
        connection_.writeRegisters(registerInfo.address, data);
    } else {
        throw util::info_exception("Unsupported register size encountered",
                                   MAKE_DEBUG_STRING());
    }
}

} /* PD */

#endif // PDAPI_HPP

