#pragma once

#include "config.hpp"

struct ParameterData {
    uint8_t version = 1;
    unsigned int count = 300U;
    unsigned int frequency = 10U; // Hz
};

class Parameter {
public:

    Parameter() : eeprom(get_eeprom()) {}

    void init() {
        if (eeprom.read(0) != data.version) {
            save();
        }
        eeprom.get(0, data);
        
        // sanity check
        if (data.frequency == 0U) {
            data.frequency = 10U; // default frequency
        }
    }

    void save() {
        eeprom.put(0, data);
        eeprom.commit();
    }

    ParameterData& get() {
        return data;
    }

private:
    ParameterData data;
    EEPROMClass& eeprom;
};