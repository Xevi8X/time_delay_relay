#pragma once

#include <cstdint>
#include <EEPROM.h>

#define PARAMETER_OFFSET 0
#define STATISTICS_OFFSET 512

static EEPROMClass& get_eeprom() {
    static EEPROMClass eeprom;
    static bool init = false;

    if (!init) {
        eeprom.begin(1024); // Initialize EEPROM with 1KB size
        init = true;
    }

    return eeprom;
}
