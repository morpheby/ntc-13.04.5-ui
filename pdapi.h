#ifndef PDAPI_H
#define PDAPI_H

#include <string>
#include <modbusconnection.h>

namespace PD {

template<typename T>
struct RegisterInfo {
    std::string name;
    std::string description;
    int address;
    size_t size;
};

class Registers {
public:
    static RegisterInfo<int> BRG_VAL;

    static RegisterInfo<float> ADC0;
    static RegisterInfo<float> ADC1;
    static RegisterInfo<float> ADC2;
    static RegisterInfo<float> ADC3;
    static RegisterInfo<float> ADC4;
    static RegisterInfo<float> ADC5;
    static RegisterInfo<float> ADC6;

    static RegisterInfo<float> K0;
    static RegisterInfo<float> K1;
    static RegisterInfo<float> K2;
    static RegisterInfo<float> K3;
    static RegisterInfo<float> K4;
    static RegisterInfo<float> K5;
    static RegisterInfo<float> K6;

    static RegisterInfo<int> OFS_ADC0;
    static RegisterInfo<int> OFS_ADC1;
    static RegisterInfo<int> OFS_ADC2;
    static RegisterInfo<int> OFS_ADC3;
    static RegisterInfo<int> OFS_ADC4;
    static RegisterInfo<int> OFS_ADC5;
    static RegisterInfo<int> OFS_ADC6;

    static RegisterInfo<int> n_a;
    static RegisterInfo<int> n_b;
    static RegisterInfo<int> n_c;

    static RegisterInfo<int> N;

    static RegisterInfo<int> D_In;
    static RegisterInfo<int> Ind_Delay;

    static RegisterInfo<int> profile;
    static RegisterInfo<int> ind_0_1;
    static RegisterInfo<int> ind_1_1;
    static RegisterInfo<int> ind_2_1;
    static RegisterInfo<int> ind_0_2;
    static RegisterInfo<int> ind_1_2;
    static RegisterInfo<int> ind_2_2;
    static RegisterInfo<int> ind_0_3;
    static RegisterInfo<int> ind_1_3;
    static RegisterInfo<int> ind_2_3;
    static RegisterInfo<int> ind_0_4;
    static RegisterInfo<int> ind_1_4;
    static RegisterInfo<int> ind_2_4;

    static RegisterInfo<float> P1;
    static RegisterInfo<float> P2;
    static RegisterInfo<float> P3;

    static RegisterInfo<float> Q1;
    static RegisterInfo<float> Q2;
    static RegisterInfo<float> Q3;

    static RegisterInfo<float> S1;
    static RegisterInfo<float> S2;
    static RegisterInfo<float> S3;

    static RegisterInfo<float> cos_f1;
    static RegisterInfo<float> cos_f2;
    static RegisterInfo<float> cos_f3;

    static RegisterInfo<float> AD4_AVG;
    static RegisterInfo<float> AD5_AVG;
    static RegisterInfo<float> AD6_AVG;

    static RegisterInfo<float> AD4_RMS;
    static RegisterInfo<float> AD5_RMS;
    static RegisterInfo<float> AD6_RMS;

    static RegisterInfo<int> FLASH_WR;
    static RegisterInfo<int> FLASH_RD;
    static RegisterInfo<int> PROF_CHANGE_SOURCE;

    static RegisterInfo<unsigned int> A0;
    static RegisterInfo<unsigned int> A1;
    static RegisterInfo<unsigned int> A2;
    static RegisterInfo<unsigned int> A3;
    static RegisterInfo<unsigned int> A4;
    static RegisterInfo<unsigned int> A5;
    static RegisterInfo<unsigned int> A6;

    static RegisterInfo<int> ind_off_1;
    static RegisterInfo<int> ind_off_2;
    static RegisterInfo<int> ind_off_3;
    static RegisterInfo<int> ind_off_4;

    static RegisterInfo<int> channel_num_U;
    static RegisterInfo<int> imp_kol;
    static RegisterInfo<int> V;
    static RegisterInfo<int> D_Out_Init;
    static RegisterInfo<int> AD_12b;
};

class PdApi
{
    ModBus::ModBusConnection connection_;
public:
    PdApi(const ModBus::ModBusConnection &connection);

    template<typename T>
    T readRegister(const RegisterInfo<T>& registerInfo);

    template<typename T>
    void writeRegister(const RegisterInfo<T>& registerInfo, const T& value);

    ModBus::ModBusConnection connection() const;
};

}

#include "pdapi.hpp"

#endif // PDAPI_H
