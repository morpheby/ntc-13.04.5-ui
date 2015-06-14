#include "pdapi.h"

using namespace PD;

RegisterInfo<int> Registers::BRG_VAL = {"BRG_VAL", "", 0, 1};

RegisterInfo<float> Registers::ADC0 = {"ADC0", "", 1, 2};
RegisterInfo<float> Registers::ADC1 = {"ADC1", "", 3, 2};
RegisterInfo<float> Registers::ADC2 = {"ADC2", "", 5, 2};
RegisterInfo<float> Registers::ADC3 = {"ADC3", "", 7, 2};
RegisterInfo<float> Registers::ADC4 = {"ADC4", "", 9, 2};
RegisterInfo<float> Registers::ADC5 = {"ADC5", "", 11, 2};
RegisterInfo<float> Registers::ADC6 = {"ADC6", "", 13, 2};

RegisterInfo<float> Registers::K0 = {"K0", "", 15, 2};
RegisterInfo<float> Registers::K1 = {"K1", "", 17, 2};
RegisterInfo<float> Registers::K2 = {"K2", "", 19, 2};
RegisterInfo<float> Registers::K3 = {"K3", "", 21, 2};
RegisterInfo<float> Registers::K4 = {"K4", "", 23, 2};
RegisterInfo<float> Registers::K5 = {"K5", "", 25, 2};
RegisterInfo<float> Registers::K6 = {"K6", "", 27, 2};

RegisterInfo<int> Registers::OFS_ADC0 = {"OFS_ADC0", "", 29, 1};
RegisterInfo<int> Registers::OFS_ADC1 = {"OFS_ADC1", "", 30, 1};
RegisterInfo<int> Registers::OFS_ADC2 = {"OFS_ADC2", "", 31, 1};
RegisterInfo<int> Registers::OFS_ADC3 = {"OFS_ADC3", "", 32, 1};
RegisterInfo<int> Registers::OFS_ADC4 = {"OFS_ADC4", "", 33, 1};
RegisterInfo<int> Registers::OFS_ADC5 = {"OFS_ADC5", "", 34, 1};
RegisterInfo<int> Registers::OFS_ADC6 = {"OFS_ADC6", "", 35, 1};

RegisterInfo<int> Registers::n_a = {"n_a", "", 36, 1};
RegisterInfo<int> Registers::n_b = {"n_b", "", 37, 1};
RegisterInfo<int> Registers::n_c = {"n_c", "", 38, 1};
//    //int ind_off __attribute__ ((address(RAM_START_ADDRESS+78)));        //REG 39

RegisterInfo<int> Registers::N = {"N", "", 40, 1};

RegisterInfo<int> Registers::D_In = {"D_In", "", 41, 1};
//    //int D_Out __attribute__ ((address(RAM_START_ADDRESS+84)));          //REG 42
RegisterInfo<int> Registers::Ind_Delay = {"Ind_Delay", "", 43, 1};

RegisterInfo<int> Registers::profile = {"profile", "", 44, 1};
RegisterInfo<int> Registers::ind_0_1 = {"ind_0_1", "", 45, 1};
RegisterInfo<int> Registers::ind_1_1 = {"ind_1_1", "", 46, 1};
RegisterInfo<int> Registers::ind_2_1 = {"ind_2_1", "", 47, 1};
RegisterInfo<int> Registers::ind_0_2 = {"ind_0_2", "", 48, 1};
RegisterInfo<int> Registers::ind_1_2 = {"ind_1_2", "", 49, 1};
RegisterInfo<int> Registers::ind_2_2 = {"ind_2_2", "", 50, 1};
RegisterInfo<int> Registers::ind_0_3 = {"ind_0_3", "", 51, 1};
RegisterInfo<int> Registers::ind_1_3 = {"ind_1_3", "", 52, 1};
RegisterInfo<int> Registers::ind_2_3 = {"ind_2_3", "", 53, 1};
RegisterInfo<int> Registers::ind_0_4 = {"ind_0_4", "", 54, 1};
RegisterInfo<int> Registers::ind_1_4 = {"ind_1_4", "", 55, 1};
RegisterInfo<int> Registers::ind_2_4 = {"ind_2_4", "", 56, 1};

RegisterInfo<float> Registers::P1 = {"P1", "", 57, 1};
RegisterInfo<float> Registers::P2 = {"P2", "", 59, 1};
RegisterInfo<float> Registers::P3 = {"P3", "", 61, 1};

RegisterInfo<float> Registers::Q1 = {"Q1", "", 63, 1};
RegisterInfo<float> Registers::Q2 = {"Q2", "", 65, 1};
RegisterInfo<float> Registers::Q3 = {"Q3", "", 67, 1};

RegisterInfo<float> Registers::S1 = {"S1", "", 69, 1};
RegisterInfo<float> Registers::S2 = {"S2", "", 71, 1};
RegisterInfo<float> Registers::S3 = {"S3", "", 73, 1};

RegisterInfo<float> Registers::cos_f1 = {"cos_f1", "", 75, 1};
RegisterInfo<float> Registers::cos_f2 = {"cos_f2", "", 77, 1};
RegisterInfo<float> Registers::cos_f3 = {"cos_f3", "", 79, 1};

RegisterInfo<float> Registers::AD4_AVG = {"AD4_AVG", "", 81, 1};
RegisterInfo<float> Registers::AD5_AVG = {"AD5_AVG", "", 83, 1};
RegisterInfo<float> Registers::AD6_AVG = {"AD6_AVG", "", 85, 1};

RegisterInfo<float> Registers::AD4_RMS = {"AD4_RMS", "", 87, 1};
RegisterInfo<float> Registers::AD5_RMS = {"AD5_RMS", "", 89, 1};
RegisterInfo<float> Registers::AD6_RMS = {"AD6_RMS", "", 91, 1};

RegisterInfo<int> Registers::FLASH_WR = {"FLASH_WR", "", 93, 1};
RegisterInfo<int> Registers::FLASH_RD = {"FLASH_RD", "", 94, 1};
RegisterInfo<int> Registers::PROF_CHANGE_SOURCE = {"PROF_CHANGE_SOURCE", "", 95, 1};

RegisterInfo<unsigned int> Registers::A0 = {"A0", "", 96, 1};
RegisterInfo<unsigned int> Registers::A1 = {"A1", "", 97, 1};
RegisterInfo<unsigned int> Registers::A2 = {"A2", "", 98, 1};
RegisterInfo<unsigned int> Registers::A3 = {"A3", "", 99, 1};
RegisterInfo<unsigned int> Registers::A4 = {"A4", "", 100, 1};
RegisterInfo<unsigned int> Registers::A5 = {"A5", "", 101, 1};
RegisterInfo<unsigned int> Registers::A6 = {"A6", "", 102, 1};

RegisterInfo<int> Registers::ind_off_1 = {"ind_off_1", "", 103, 1};
RegisterInfo<int> Registers::ind_off_2 = {"ind_off_2", "", 104, 1};
RegisterInfo<int> Registers::ind_off_3 = {"ind_off_3", "", 105, 1};
RegisterInfo<int> Registers::ind_off_4 = {"ind_off_4", "", 106, 1};

RegisterInfo<int> Registers::channel_num_U = {"channel_num_U", "", 107, 1};
RegisterInfo<int> Registers::imp_kol = {"imp_kol", "", 108, 1};
RegisterInfo<int> Registers::V = {"V", "", 109, 1};
RegisterInfo<int> Registers::D_Out_Init = {"D_Out_Init", "", 110, 1};
RegisterInfo<int> Registers::AD_12b = {"AD_12b", "", 111, 1};


PdApi::PdApi(const ModBus::ModBusConnection &connection) :
    connection_{connection} {
}

ModBus::ModBusConnection PdApi::connection() const {
    return this->connection_;
}

