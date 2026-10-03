
#ifndef UTILS_H
#define UTILS_H

#include <stdint.h>

#define Clamp(value, min, max) _Generic((value), \
    int8_t: ClampInt8,                           \
    uint8_t: ClampUInt8,                         \
    int16_t: ClampInt16,                         \
    uint16_t: ClampUInt16,                       \
    int32_t: ClampInt32,                         \
    uint32_t: ClampUInt32,                       \
    int64_t: ClampInt64,                         \
    uint64_t: ClampUInt64,                       \
    float: ClampFloat,                           \
    double: ClampDouble)(value, min, max)

typedef struct Direction
{
    int x;
    int y;
} Direction;

int ClampInt(int value, int min, int max);
unsigned int ClampUInt(unsigned int value,
                       unsigned int min,
                       unsigned int max);

int8_t ClampInt8(int8_t value, int8_t min, int8_t max);
uint8_t ClampUInt8(uint8_t value, uint8_t min, uint8_t max);
int16_t ClampInt16(int16_t value, int16_t min, int16_t max);
uint16_t ClampUInt16(uint16_t value, uint16_t min, uint16_t max);
int32_t ClampInt32(int32_t value, int32_t min, int32_t max);
uint32_t ClampUInt32(uint32_t value, uint32_t min, uint32_t max);
int64_t ClampInt64(int64_t value, int64_t min, int64_t max);
uint64_t ClampUInt64(uint64_t value, uint64_t min, uint64_t max);
float ClampFloat(float value, float min, float max);
double ClampDouble(double value, double min, double max);

#endif
