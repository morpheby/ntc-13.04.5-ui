#ifndef PDAPI_HPP
#define PDAPI_HPP

#include <array>
#include "serial/Logger.h"

namespace PD {

template<typename T>
T PdApi::readRegister(const RegisterInfo<T>& registerInfo) {
    if (registerInfo.size == 1) {
        std::array<uint16_t, 1> data = connection_.readRegisters<1>(registerInfo.address);
        std::array<uint16_t, sizeof(T)/2> tmp;
        std::fill(tmp.begin(), tmp.end(), 0);
        tmp[0] = data[0];
        return T(*((T*)tmp.data()));
    } else if (registerInfo.size == 2) {
        std::array<uint16_t, 2> data = connection_.readRegisters<2>(registerInfo.address);
        std::array<uint16_t, sizeof(T)/2> tmp;
        std::fill(tmp.begin(), tmp.end(), 0);
        tmp[0] = data[0];
        tmp[1] = data[1];
        return T(*((T*)tmp.data()));
    } else {
        throw util::info_exception("Unsupported register size encountered",
                                   MAKE_DEBUG_STRING());
    }
}

template<typename T>
void PdApi::writeRegister(const RegisterInfo<T>& registerInfo, const T& value) {
    if (registerInfo.size == 1) {
        std::array<uint16_t, 1> data;
        *(data.data()) = *((uint16_t*) &value);
        connection_.writeRegisters<1>(registerInfo.address, data);
    } else if (registerInfo.size == 2) {
        std::array<uint16_t, 2> data;
        *(data.data()) = *((uint16_t*) &value);
        *(data.data()+1) = *(((uint16_t*) &value)+1);
        connection_.writeRegisters<2>(registerInfo.address, data);
    } else {
        throw util::info_exception("Unsupported register size encountered",
                                   MAKE_DEBUG_STRING());
    }
}

} /* PD */

#endif // PDAPI_HPP

