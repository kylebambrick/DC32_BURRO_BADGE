#include "burro_mem.h"
#include <Arduino.h>

Flags flags;

#define EEPROM_SIZE 512
#define MAGIC_ADDR 0
#define MAGIC_VALUE 0x42

void initEEPROM() {
    EEPROM.begin(EEPROM_SIZE);
    if (EEPROM.read(MAGIC_ADDR) != MAGIC_VALUE) {
        resetEEPROM();
    }
    loadFlags();
}


void loadFlags() {
    EEPROM.get(1, flags.colorID);
    flags.flagSGS = EEPROM.read(5);
    flags.flagKyle = EEPROM.read(6);
    flags.flagUser = EEPROM.read(7);
    flags.flagPass = EEPROM.read(8);
    flags.flagLogin = EEPROM.read(9);
    flags.flagUnlockIt = EEPROM.read(10);
}

void setFlag(String flag) {
    if (flag == "SGS") {
        EEPROM.write(5, true);
        flags.flagSGS = true;
    } else if (flag == "blametheotherkyle") {
        flags.flagKyle = true;
        EEPROM.write(6, true);
    } else if (flag == "the_username_is_burroadmin") {
        flags.flagUser = true;
        EEPROM.write(7, true);
    } else if (flag == "sUPErZ3cR3t1337PA5SW0Rd!" || flag == "sUPErZ3cR3t1337PA5SW0Rd%21") {
        EEPROM.write(8, true);
        flags.flagPass = true;
    } else if (flag == "Ic@nL0g1n!" || flag == "Ic%40nL0g1n%21") {
        EEPROM.write(9, true);
        flags.flagLogin = true;
    } else if (flag == "unlock_it") {
        EEPROM.write(10, true);
        flags.flagUnlockIt = true;
    }
    EEPROM.commit();
}

void saveColor(int id) {
    EEPROM.put(1, id);
    EEPROM.commit();
    int readId;
    EEPROM.get(1, readId);
}

void resetEEPROM() {
    for (int i = 0; i < EEPROM_SIZE; i++) {
        EEPROM.write(i, 0);
    }
    EEPROM.write(1, 1);
    EEPROM.write(MAGIC_ADDR, MAGIC_VALUE);
    EEPROM.commit();
}