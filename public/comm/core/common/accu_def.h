
#ifndef ACCU_DEF_H_
#define ACCU_DEF_H_

#include <stdint.h>
#include <stdlib.h>
#include <core/base/v8stdint.h>
#include <core/base/typedef.h>



void EndianSwap(std::uint8_t *_data, int _start_index, int _length) ;
float u8Arry2float(std::uint8_t *data, bool key= false);
void float2u8Arry(std::uint8_t *u8Arry, float *floatdata, bool key = false);
int u8Arry2Int(std::uint8_t *b);

#endif  // ACCU_DEF_H_
