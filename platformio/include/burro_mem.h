#ifndef BURRO_MEM_H
#define BURRO_MEM_H

#include <EEPROM.h>

struct Flags {
    int colorID;
    bool flagSGS;
    bool flagKyle;
    bool flagUser;
    bool flagPass;
    bool flagLogin;
    bool flagUnlockIt;
};

extern Flags flags;

void initEEPROM();
void loadFlags();
void setFlag(String flag);
void saveColor(int id);
void resetEEPROM();

#endif // BURRO_MEM_H
