#include <MemoryFree.h>
#include <TimeLib.h>
#include <DS1307RTC.h>
#include <EEPROM.h>
#include <LiquidCrystal.h>

#define AI_B1_CODE 785
#define AI_B2_CODE 853
#define AI_B3_CODE 930
#define AI_B4_CODE 1023

#define AI_ACCURACY 10

#define AI_DELAY 200

enum AI_BUTTONS { AI_B1, AI_B2, AI_B3, AI_B4 };
