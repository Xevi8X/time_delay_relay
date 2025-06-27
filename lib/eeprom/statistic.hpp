#pragma once

#include "config.hpp"

struct StatisticData {
    uint64_t boot_count = 0UL;
    uint64_t reset_button_count = 0UL;
};

class Statistic {
public:

    Statistic() : eeprom(get_eeprom()) {}

    void init() {
        eeprom.get(STATISTICS_OFFSET, data);
        data.boot_count++;
        eeprom.put(STATISTICS_OFFSET + offsetof(StatisticData, boot_count), data.boot_count);
        eeprom.commit();
    }

    void bump_reset_button_count() {
        data.reset_button_count++;
        eeprom.put(STATISTICS_OFFSET + offsetof(StatisticData, reset_button_count), data.reset_button_count);
        eeprom.commit();
    }

    const StatisticData& get() {
        return data;
    }

    void reset() {
        data.boot_count = 0UL;
        data.reset_button_count = 0UL;
        eeprom.put(STATISTICS_OFFSET, data);
        eeprom.commit();
    }

private:
    StatisticData data;
    EEPROMClass& eeprom;
};
