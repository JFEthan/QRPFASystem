
#include <core/common/accu_def.h>
#include <string.h>
#include <core/base/v8stdint.h>

void EndianSwap(std::uint8_t *_data, int _start_index, int _length) {
  int _i, _cnt, _end, _start;
  _cnt = _length / 2;
  _start = _start_index;
  _end = _start_index + _length - 1;
  std::uint8_t _tmp;
  for (_i = 0; _i < _cnt; _i++)
  {
    _tmp = _data[_start + _i];
    _data[_start + _i] = _data[_end - _i];
    _data[_end - _i] = _tmp;
  }
}

float u8Arry2float(std::uint8_t *data, bool key)
{
    float fa = 0;
    std::uint8_t uc[4];
    if (key == false)
    {
        uc[3] = data[0];
        uc[2] = data[1];
        uc[1] = data[2];
        uc[0] = data[3];
    }
    else
    {
        uc[0] = data[0];
        uc[1] = data[1];
        uc[2] = data[2];
        uc[3] = data[3];
    }

    memcpy(&fa, uc, 4);
    return fa;
}

void float2u8Arry(std::uint8_t *u8Arry, float *floatdata, bool key)
{
    std::uint8_t farray[4];
    *(float *)farray = *floatdata;
    if (key == false)
    {
        u8Arry[3] = farray[0];
        u8Arry[2] = farray[1];
        u8Arry[1] = farray[2];
        u8Arry[0] = farray[3];
    }
    else
    {
        u8Arry[0] = farray[0];
        u8Arry[1] = farray[1];
        u8Arry[2] = farray[2];
        u8Arry[3] = farray[3];
    }
}

int u8Arry2Int(std::uint8_t *b)
{
    return b[0]<<24 | b[1]<<16 | b[2]<<8 | b[3];
}