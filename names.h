#ifndef __NAMES_H__
#define __NAMES_H__

const bool name_close[64] PROGMEM = {
    0, 0, 0, 0, 0, 0, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 0, 0, 0, 0, 0, 0, 0
};

const bool name_diffA[64] PROGMEM = {
    0, 0, 0, 1, 1, 0, 0, 0,
    0, 0, 1, 0, 0, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 0, 0, 1, 1, 1
};

const bool name_feedA[64] PROGMEM = {
    0, 0, 0, 1, 1, 0, 1, 0,
    0, 0, 1, 0, 0, 1, 1, 1,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 0, 0, 1, 1, 1
};

const bool name_dripA[64] PROGMEM = {
    0, 0, 0, 1, 1, 0, 0, 1,
    0, 0, 1, 0, 0, 1, 0, 1,
    0, 1, 0, 0, 0, 0, 1, 1,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    1, 1, 1, 0, 0, 1, 1, 1
};

const bool name_diffB[64] PROGMEM = {
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 0, 0, 0, 0, 1, 0,
    0, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0
};

const bool name_killB[64] PROGMEM = {
    1, 1, 1, 1, 1, 1, 0, 0,
    0, 1, 0, 0, 0, 1, 1, 1,
    0, 1, 0, 0, 0, 1, 0, 0,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0
};

const bool name_dripB[64] PROGMEM = {
    1, 1, 1, 1, 1, 1, 0, 1,
    0, 1, 0, 0, 0, 0, 1, 1,
    0, 1, 0, 0, 0, 1, 0, 1,
    0, 1, 1, 1, 1, 1, 1, 0,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    0, 1, 0, 0, 0, 0, 0, 1,
    1, 1, 1, 1, 1, 1, 1, 0
};

#endif // __NAMES_H__
