#ifndef __FONTS_H__
#define __FONTS_H__

#include <stdint.h>

typedef struct
{
    const uint8_t width;
    uint8_t height;
    const uint16_t *data;
} FontDef;


extern FontDef Font_7x10;
extern FontDef Font_11x18;
extern FontDef Font_16x26;


extern const uint16_t saber[][128];
extern const uint16_t knky[][240];
extern const uint16_t tek[][240];


#endif
