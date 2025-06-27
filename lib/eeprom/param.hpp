#pragma once

#include "config.hpp"

struct ParameterData {
    uint8_t version = 1;
    unsigned int count = 300U;
    unsigned int frequency = 10U; // Hz
    unsigned int boot_delay_ms = 500U; // ms
};

class Parameter {
public:

    Parameter() : eeprom(get_eeprom()) {}

    void init() {
        // if (eeprom.read(0) != data.version) {
        //     save();
        // }
        // eeprom.get(0, data);

        // // sanity check
        // if (data.count < 300U || data.count > 1200U) {
        //     data.count = 300U; // at least 5 minutes
        // }
        
        // if (data.frequency < 5U || data.frequency > 100U) {
        //     data.frequency = 10U; // default frequency
        // }

        // if (data.boot_delay_ms > 3000U) {
        //     data.boot_delay_ms = 500U; // default boot delay
        // }
    }

    void save() {
        // eeprom.put(0, data);
        // eeprom.commit();
    }

    ParameterData& get() {
        return data;
    }

private:
    ParameterData data;
    EEPROMClass& eeprom;
};