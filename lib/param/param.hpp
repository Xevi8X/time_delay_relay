#pragma once

#include <cstdint>
#include <EEPROM.h>

struct ParameterData {
    uint8_t version = 1;
    unsigned int count = 300U;
    unsigned int frequency = 10U; // Hz
};

class Parameter {
public:

    void init() {
        eeprom.begin(sizeof(ParameterData));
        if (eeprom.read(0) != data.version) {
            save();
        }
        eeprom.get(0, data);
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
    EEPROMClass eeprom;
};