#pragma once

#include <cstdint>
#include <EEPROM.h>

#define PARAMETER_OFFSET 0
#define STATISTICS_OFFSET 64

static EEPROMClass& get_eeprom() {
    static EEPROMClass eeprom;
    static bool init = false;

    if (!init) {
        eeprom.begin(STATISTICS_OFFSET * 2);
        init = true;
    }

    return eeprom;
}
